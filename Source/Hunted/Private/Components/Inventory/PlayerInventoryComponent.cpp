// KasaiRaito All Rights Reserved


#include "Components/Inventory/PlayerInventoryComponent.h"

#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"
#include "Widget/Inventory/PlayerInventoryGridWidget.h"

#include "HuntedDebugHelper.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

bool UPlayerInventoryComponent::IsTileValid(FIntPoint Tile) const
{
	if (Tile.X >= 0 && Tile.Y >= 0  && Tile.X < Columns && Tile.Y < Rows)
	{
		return true;
	}
	
	return false;
}

bool UPlayerInventoryComponent::RoomForItemInInventoryIgnoringItem(const AHuntedInventoryItemBase* ItemToAdd,
	int8 TopLeftIndex, const AHuntedInventoryItemBase* IgnoredItem) const
{
	if (!ItemToAdd)
	{
		return false;
	}

	const FIntPoint Dimensions = ItemToAdd->GetItemInventorySize();
	const FIntPoint Tile = IndexToTile(TopLeftIndex);

	for (int16 i = Tile.X; i <= (Tile.X + Dimensions.X - 1); i++)
	{
		for (int16 j = Tile.Y; j <= (Tile.Y + Dimensions.Y - 1); j++)
		{
			const FIntPoint CurrentTile(i, j);
			if (!IsTileValid(CurrentTile))
			{
				return false;
			}

			const int8 Index = TileToIndex(CurrentTile);
			if (!GetResultAtIndex(Index))
			{
				return false;
			}

			AHuntedInventoryItemBase* OccupyingItem = Items[Index];
			if (OccupyingItem && OccupyingItem != IgnoredItem)
			{
				return false;
			}
		}
	}

	return true;
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	//Debug::Print(TEXT("Player Inventory Component BeginPlay"), FColor::Purple);
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TMap<AHuntedInventoryItemBase*, FIntPoint> UPlayerInventoryComponent::GetAllItems()
{
	AllItems.Reset();
	
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i])
		{
			if (!AllItems.Contains(Items[i]))
			{
				AllItems.Add(Items[i], IndexToTile(static_cast<int8>(i)));
			}
		}
	}
	
	return AllItems;
}

bool UPlayerInventoryComponent::TryAddItem(AHuntedInventoryItemBase* ItemToAdd)
{
	if (!ItemToAdd)
	{
		Debug::Print(TEXT("Player Inventory Component - Try To Add Item: No Item To Add reference on call"), FColor::Red);
		return false;
	}

	const bool bCanUseStacking = ItemToAdd->IsItemStackable()
		&& ItemToAdd->GetItemInventorySize() == FIntPoint(1, 1)
		&& ItemToAdd->GetMaxStackPerCell() > 0;

	if (!bCanUseStacking)
	{
		for (int8 i = 0; i < Items.Num(); i++)
		{
			if (RoomForItemInInventory(ItemToAdd, i))
			{
				AddItemAtIndex(ItemToAdd, i);
				return true;
			}
		}
	
		Debug::Print(TEXT("Player Inventory Component - Try To Add Item: No Room in Inventory"), FColor::Red);
		return false;
	}

	int32 RemainingAmount = FMath::Max(1, ItemToAdd->GetItemAmount());
	ItemToAdd->SetItemAmount(RemainingAmount);
	const int32 MaxStackPerCell = FMath::Max(1, ItemToAdd->GetMaxStackPerCell());

	// Fill compatible stacks that already exist in the inventory.
	const TMap<AHuntedInventoryItemBase*, FIntPoint> ExistingItems = GetAllItems();
	for (const TPair<AHuntedInventoryItemBase*, FIntPoint>& Pair : ExistingItems)
	{
		AHuntedInventoryItemBase* ExistingStack = Pair.Key;
		if (!CanItemsStackTogether(ItemToAdd, ExistingStack))
		{
			continue;
		}

		const int32 ExistingAmount = ExistingStack->GetItemAmount();
		const int32 ExistingMax = FMath::Max(1, ExistingStack->GetMaxStackPerCell());
		const int32 Capacity = FMath::Max(0, ExistingMax - ExistingAmount);
		if (Capacity <= 0)
		{
			continue;
		}

		const int32 TransferAmount = FMath::Min(Capacity, RemainingAmount);
		ExistingStack->AddItemAmount(TransferAmount);
		RemainingAmount -= TransferAmount;

		if (RemainingAmount <= 0)
		{
			ItemToAdd->SetItemAmount(0);
			RefreshInventoryGrid();
			return true;
		}
	}

	// Keep the original item as the final residue holder, and place full stacks using clones first.
	for (int8 Index = 0; Index < Items.Num() && RemainingAmount > MaxStackPerCell; Index++)
	{
		if (!RoomForItemInInventory(ItemToAdd, Index))
		{
			continue;
		}

		AHuntedInventoryItemBase* NewStackItem = SpawnStackCloneFromItem(ItemToAdd);
		if (!NewStackItem)
		{
			break;
		}

		NewStackItem->SetItemAmount(MaxStackPerCell);
		AddItemAtIndex(NewStackItem, Index);
		RemainingAmount -= MaxStackPerCell;
	}

	// Place the original item if there is an available cell for the final remainder.
	for (int8 Index = 0; Index < Items.Num() && RemainingAmount > 0; Index++)
	{
		if (!RoomForItemInInventory(ItemToAdd, Index))
		{
			continue;
		}

		ItemToAdd->SetItemAmount(FMath::Min(RemainingAmount, MaxStackPerCell));
		AddItemAtIndex(ItemToAdd, Index);
		RemainingAmount -= ItemToAdd->GetItemAmount();
		break;
	}

	if (RemainingAmount > 0)
	{
		ItemToAdd->SetItemAmount(RemainingAmount);
		Debug::Print(TEXT("Player Inventory Component - Try To Add Item: Partial stack, no more room"), FColor::Yellow);
		RefreshInventoryGrid();
		return false;
	}
	
	return true;
}

int32 UPlayerInventoryComponent::GetTotalItemAmountByTag(FGameplayTag ItemTag) const
{
	if (!ItemTag.IsValid())
	{
		return 0;
	}

	int32 TotalAmount = 0;
	TSet<const AHuntedInventoryItemBase*> CountedItems;

	for (AHuntedInventoryItemBase* Item : Items)
	{
		if (!Item || CountedItems.Contains(Item))
		{
			continue;
		}

		CountedItems.Add(Item);

		const FHuntedPlayerItemData ItemData = Item->GetItemData();
		if (ItemData.ItemTag != ItemTag)
		{
			continue;
		}

		TotalAmount += Item->IsItemStackable() ? FMath::Max(0, Item->GetItemAmount()) : 1;
	}

	return TotalAmount;
}

int32 UPlayerInventoryComponent::GetAvailableItemAmountByTag(FGameplayTag ItemTag) const
{
	return GetTotalItemAmountByTag(ItemTag);
}

bool UPlayerInventoryComponent::HasItemAmountByTag(FGameplayTag ItemTag, int32 RequiredAmount) const
{
	if (RequiredAmount <= 0)
	{
		return false;
	}

	return GetTotalItemAmountByTag(ItemTag) >= RequiredAmount;
}

bool UPlayerInventoryComponent::TryRemoveItemAmountByTag(FGameplayTag ItemTag, int32 AmountToRemove)
{
	if (!ItemTag.IsValid() || AmountToRemove <= 0)
	{
		return false;
	}

	if (!HasItemAmountByTag(ItemTag, AmountToRemove))
	{
		return false;
	}

	int32 RemainingAmountToRemove = AmountToRemove;
	TSet<AHuntedInventoryItemBase*> ProcessedItems;

	for (AHuntedInventoryItemBase* Item : Items)
	{
		if (!Item || ProcessedItems.Contains(Item))
		{
			continue;
		}

		ProcessedItems.Add(Item);

		const FHuntedPlayerItemData ItemData = Item->GetItemData();
		if (ItemData.ItemTag != ItemTag)
		{
			continue;
		}

		const int32 ItemAmount = Item->IsItemStackable() ? FMath::Max(0, Item->GetItemAmount()) : 1;
		if (ItemAmount <= 0)
		{
			continue;
		}

		const int32 ConsumedAmount = FMath::Min(ItemAmount, RemainingAmountToRemove);
		const int32 NewAmount = ItemAmount - ConsumedAmount;
		RemainingAmountToRemove -= ConsumedAmount;

		if (Item->IsItemStackable() && NewAmount > 0)
		{
			Item->SetItemAmount(NewAmount);
		}
		else
		{
			RemoveItem(Item);
			Item->Destroy();
		}

		if (RemainingAmountToRemove <= 0)
		{
			RefreshInventoryGrid();
			return true;
		}
	}

	RefreshInventoryGrid();
	return false;
}

bool UPlayerInventoryComponent::RoomForItemInInventory(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex) const
{
	return RoomForItemInInventoryIgnoringItem(ItemToAdd, TopLeftIndex, nullptr);
}

FIntPoint UPlayerInventoryComponent::IndexToTile(int8 Index) const
{
	return FIntPoint(Index % Columns, Index / Columns);
}

int8 UPlayerInventoryComponent::TileToIndex(FIntPoint Tile) const
{
	int8 Index = Tile.X + Tile.Y * Columns;
	return Index;
}

bool UPlayerInventoryComponent::GetResultAtIndex(int8 Index) const
{
	if (Items.IsValidIndex(Index))
	{
		return true;
	}
	
	return false;
}

AHuntedInventoryItemBase* UPlayerInventoryComponent::GetItemAtIndex(int8 Index)
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index];
	}

	return nullptr;
}

bool UPlayerInventoryComponent::FindItemTopLeftTile(AHuntedInventoryItemBase* ItemToFind, FIntPoint& OutTopLeftTile) const
{
	if (!ItemToFind)
	{
		return false;
	}

	for (int32 Index = 0; Index < Items.Num(); Index++)
	{
		if (Items[Index] == ItemToFind)
		{
			OutTopLeftTile = IndexToTile(static_cast<int8>(Index));
			return true;
		}
	}

	return false;
}

void UPlayerInventoryComponent::AddItemAtIndex(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex)
{
	if (!ItemToAdd)
	{
		return;
	}

	FIntPoint Dimensions = ItemToAdd->GetItemInventorySize();
	FIntPoint Tile = IndexToTile(TopLeftIndex);
	
	for (int16 i = Tile.X; i <= (Tile.X + Dimensions.X -1); i++)
	{
		for (int16 j = Tile.Y; j <= (Tile.Y + Dimensions.Y -1); j++)
		{
			Items[TileToIndex(FIntPoint(i, j))] = ItemToAdd;
		}
	}
	
	AddedItem = true;

	RefreshInventoryGrid();
}

void UPlayerInventoryComponent::RemoveItem(AHuntedInventoryItemBase* ItemToRemove)
{
	if (!ItemToRemove)
	{
		return;
	}
	
	for (int16 i = 0; i < Items.Num(); i++)
	{
		if (ItemToRemove == Items[i])
		{
			Items[i] = nullptr;
		}
	}
}

bool UPlayerInventoryComponent::CanItemsStackTogether(const AHuntedInventoryItemBase* SourceItem,
	const AHuntedInventoryItemBase* TargetItem) const
{
	if (!SourceItem || !TargetItem || SourceItem == TargetItem)
	{
		return false;
	}

	if (!SourceItem->IsItemStackable() || !TargetItem->IsItemStackable())
	{
		return false;
	}

	const FHuntedPlayerItemData SourceData = SourceItem->GetItemData();
	const FHuntedPlayerItemData TargetData = TargetItem->GetItemData();

	if (SourceData.ItemTag.IsValid() || TargetData.ItemTag.IsValid())
	{
		return SourceData.ItemTag == TargetData.ItemTag;
	}

	return SourceItem->GetClass() == TargetItem->GetClass();
}

bool UPlayerInventoryComponent::CanStackItemAtIndex(const AHuntedInventoryItemBase* SourceItem, int8 TopLeftIndex) const
{
	if (!SourceItem || !GetResultAtIndex(TopLeftIndex))
	{
		return false;
	}

	AHuntedInventoryItemBase* TargetItem = Items[TopLeftIndex];
	if (!CanItemsStackTogether(SourceItem, TargetItem))
	{
		return false;
	}

	const int32 SourceAmount = SourceItem->GetItemAmount();
	const int32 TargetAmount = TargetItem->GetItemAmount();
	const int32 TargetMax = FMath::Max(1, TargetItem->GetMaxStackPerCell());
	return SourceAmount > 0 && TargetAmount < TargetMax;
}

AHuntedInventoryItemBase* UPlayerInventoryComponent::SpawnStackCloneFromItem(const AHuntedInventoryItemBase* SourceItem) const
{
	if (!SourceItem || !GetWorld())
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AHuntedInventoryItemBase* NewStackItem = GetWorld()->SpawnActor<AHuntedInventoryItemBase>(
		SourceItem->GetClass(),
		FVector(0.0f, 0.0f, -50000.0f),
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (!NewStackItem)
	{
		return nullptr;
	}

	NewStackItem->SetItemInventorySize(SourceItem->GetItemInventorySize());
	NewStackItem->SetIcon(SourceItem->GetIcon());
	NewStackItem->SetItemData(SourceItem->GetItemData());
	NewStackItem->SetActorHiddenInGame(true);
	NewStackItem->SetActorEnableCollision(false);

	return NewStackItem;
}

void UPlayerInventoryComponent::RefreshInventoryGrid() const
{
	if (InventoryGridWidgetReference)
	{
		InventoryGridWidgetReference->RefreshItemWidgets();
	}
}

bool UPlayerInventoryComponent::CanPlaceOrStackItemAtIndex(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex) const
{
	if (!ItemToAdd)
	{
		return false;
	}

	if (RoomForItemInInventory(ItemToAdd, TopLeftIndex))
	{
		return true;
	}

	return CanStackItemAtIndex(ItemToAdd, TopLeftIndex);
}

bool UPlayerInventoryComponent::CanMoveItemToIndex(AHuntedInventoryItemBase* ItemToMove, int8 TargetIndex) const
{
	if (!ItemToMove || !GetResultAtIndex(TargetIndex))
	{
		return false;
	}

	if (RoomForItemInInventoryIgnoringItem(ItemToMove, TargetIndex, ItemToMove))
	{
		return true;
	}

	return CanStackItemAtIndex(ItemToMove, TargetIndex);
}

bool UPlayerInventoryComponent::TryStackItemAtIndex(AHuntedInventoryItemBase* ItemToStack, int8 TopLeftIndex,
	bool& bOutSourceConsumed)
{
	bOutSourceConsumed = false;

	if (!CanStackItemAtIndex(ItemToStack, TopLeftIndex))
	{
		return false;
	}

	AHuntedInventoryItemBase* TargetItem = Items[TopLeftIndex];
	if (!TargetItem)
	{
		return false;
	}

	const int32 SourceAmount = ItemToStack->GetItemAmount();
	const int32 TargetAmount = TargetItem->GetItemAmount();
	const int32 TargetMax = FMath::Max(1, TargetItem->GetMaxStackPerCell());
	const int32 Capacity = FMath::Max(0, TargetMax - TargetAmount);
	const int32 TransferAmount = FMath::Min(Capacity, SourceAmount);

	if (TransferAmount <= 0)
	{
		return false;
	}

	TargetItem->AddItemAmount(TransferAmount);
	ItemToStack->AddItemAmount(-TransferAmount);
	bOutSourceConsumed = ItemToStack->GetItemAmount() <= 0;

	RefreshInventoryGrid();
	return true;
}

bool UPlayerInventoryComponent::TryMoveItemToIndex(AHuntedInventoryItemBase* ItemToMove, int TargetIndex,
	bool& bOutSourceConsumed)
{
	bOutSourceConsumed = false;

	if (!ItemToMove || !GetResultAtIndex(TargetIndex))
	{
		return false;
	}

	if (TryStackItemAtIndex(ItemToMove, TargetIndex, bOutSourceConsumed))
	{
		if (bOutSourceConsumed)
		{
			RemoveItem(ItemToMove);
			RefreshInventoryGrid();
		}

		return true;
	}

	if (!RoomForItemInInventoryIgnoringItem(ItemToMove, TargetIndex, ItemToMove))
	{
		return false;
	}

	RemoveItem(ItemToMove);
	AddItemAtIndex(ItemToMove, TargetIndex);
	return true;
}
