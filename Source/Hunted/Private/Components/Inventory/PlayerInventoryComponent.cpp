// KasaiRaito All Rights Reserved


#include "Components/Inventory/PlayerInventoryComponent.h"

#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "HuntedTypes/HuntedStructTypes.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"
#include "Widget/Inventory/PlayerInventoryGridWidget.h"

#include "HuntedDebugHelper.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	BaseBulletsCount = 0;
	
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
	return RoomForItemInGridIgnoringItem(Items, ItemToAdd, TopLeftIndex, IgnoredItem);
}

bool UPlayerInventoryComponent::RoomForItemInGridIgnoringItem(
	const TArray<AHuntedInventoryItemBase*>& GridItems, const AHuntedInventoryItemBase* ItemToAdd,
	int8 TopLeftIndex, const AHuntedInventoryItemBase* IgnoredItem) const
{
	if (!IsValid(ItemToAdd))
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
			if (!GridItems.IsValidIndex(Index))
			{
				return false;
			}

			AHuntedInventoryItemBase* OccupyingItem = GridItems[Index];
			if (IsValid(OccupyingItem) && OccupyingItem != IgnoredItem)
			{
				return false;
			}
		}
	}

	return true;
}

const TArray<AHuntedInventoryItemBase*>& UPlayerInventoryComponent::GetGridItems(
	EPlayerInventoryGridType GridType) const
{
	return GridType == EPlayerInventoryGridType::Discard ? DiscardItems : Items;
}

TArray<AHuntedInventoryItemBase*>& UPlayerInventoryComponent::GetMutableGridItems(
	EPlayerInventoryGridType GridType)
{
	return GridType == EPlayerInventoryGridType::Discard ? DiscardItems : Items;
}

bool UPlayerInventoryComponent::FindItemTopLeftTileInItems(
	const TArray<AHuntedInventoryItemBase*>& GridItems, const AHuntedInventoryItemBase* ItemToFind,
	FIntPoint& OutTopLeftTile) const
{
	if (!IsValid(ItemToFind))
	{
		return false;
	}

	for (int32 Index = 0; Index < GridItems.Num(); Index++)
	{
		if (IsValid(GridItems[Index]) && GridItems[Index] == ItemToFind)
		{
			OutTopLeftTile = IndexToTile(static_cast<int8>(Index));
			return true;
		}
	}

	return false;
}

bool UPlayerInventoryComponent::FindItemGridType(const AHuntedInventoryItemBase* ItemToFind,
	EPlayerInventoryGridType& OutGridType) const
{
	FIntPoint UnusedTile;
	if (FindItemTopLeftTileInItems(Items, ItemToFind, UnusedTile))
	{
		OutGridType = EPlayerInventoryGridType::Inventory;
		return true;
	}

	if (bOverflowResolutionActive && FindItemTopLeftTileInItems(DiscardItems, ItemToFind, UnusedTile))
	{
		OutGridType = EPlayerInventoryGridType::Discard;
		return true;
	}

	return false;
}

void UPlayerInventoryComponent::AddItemToGridAtIndex(TArray<AHuntedInventoryItemBase*>& GridItems,
	AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex, bool bPrepareForInventory)
{
	if (!IsValid(ItemToAdd) || !GridItems.IsValidIndex(TopLeftIndex))
	{
		return;
	}

	const FIntPoint Dimensions = ItemToAdd->GetItemInventorySize();
	const FIntPoint Tile = IndexToTile(TopLeftIndex);
	if (Dimensions.X <= 0 || Dimensions.Y <= 0)
	{
		return;
	}

	for (int32 X = Tile.X; X < Tile.X + Dimensions.X; X++)
	{
		for (int32 Y = Tile.Y; Y < Tile.Y + Dimensions.Y; Y++)
		{
			if (!IsTileValid(FIntPoint(X, Y)))
			{
				return;
			}
		}
	}

	if (bPrepareForInventory)
	{
		PrepareItemForInventory(ItemToAdd);
	}

	for (int32 X = Tile.X; X < Tile.X + Dimensions.X; X++)
	{
		for (int32 Y = Tile.Y; Y < Tile.Y + Dimensions.Y; Y++)
		{
			const int8 ItemIndex = TileToIndex(FIntPoint(X, Y));
			if (GridItems.IsValidIndex(ItemIndex))
			{
				GridItems[ItemIndex] = ItemToAdd;
			}
		}
	}
}

void UPlayerInventoryComponent::RemoveItemFromGrid(TArray<AHuntedInventoryItemBase*>& GridItems,
	AHuntedInventoryItemBase* ItemToRemove)
{
	if (!ItemToRemove)
	{
		return;
	}

	for (AHuntedInventoryItemBase*& GridItem : GridItems)
	{
		if (GridItem == ItemToRemove)
		{
			GridItem = nullptr;
		}
	}
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	BaseBulletsCount = 0;
	//Debug::Print(TEXT("Player Inventory Component BeginPlay"), FColor::Purple);
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerInventoryComponent::CompactInvalidInventoryItems()
{
	bool bRemovedInvalidReference = false;

	auto RemoveInvalidReferences = [&bRemovedInvalidReference](TArray<AHuntedInventoryItemBase*>& GridItems)
	{
		for (AHuntedInventoryItemBase*& Item : GridItems)
		{
			if (Item && !IsValid(Item))
			{
				// Prevents later inventory queries from dereferencing actors destroyed by pickup/drop Blueprints.
				Item = nullptr;
				bRemovedInvalidReference = true;
			}
		}
	};

	RemoveInvalidReferences(Items);
	RemoveInvalidReferences(DiscardItems);

	if (PendingCombineItem && !IsValid(PendingCombineItem))
	{
		PendingCombineItem = nullptr;
	}

	if (PendingCombineItem)
	{
		bool bPendingItemStillPresent = false;
		for (AHuntedInventoryItemBase* Item : Items)
		{
			if (Item == PendingCombineItem)
			{
				bPendingItemStillPresent = true;
				break;
			}
		}

		if (!bPendingItemStillPresent)
		{
			PendingCombineItem = nullptr;
		}
	}

	if (bRemovedInvalidReference)
	{
		AllItems.Reset();
	}
}

void UPlayerInventoryComponent::PrepareItemForInventory(AHuntedInventoryItemBase* ItemToPrepare) const
{
	if (!IsValid(ItemToPrepare))
	{
		return;
	}

	// Inventory-held actors must remain alive but not collide/render as world pickups.
	ItemToPrepare->SetOwner(GetOwner());
	ItemToPrepare->SetActorHiddenInGame(true);
	ItemToPrepare->SetActorEnableCollision(false);
	ItemToPrepare->SetActorTickEnabled(false);
}

AHuntedInventoryItemBase* UPlayerInventoryComponent::ResolveInventoryStorageItem(AHuntedInventoryItemBase* SourceItem)
{
	if (!IsValid(SourceItem))
	{
		return nullptr;
	}

	if (IsItemInInventory(SourceItem) || SourceItem->GetOwner() == GetOwner() || SourceItem->IsHidden())
	{
		PrepareItemForInventory(SourceItem);
		return SourceItem;
	}

	// Pickup Blueprints often destroy their world actor after TryAddItem; store a hidden clone instead.
	return SpawnStackCloneFromItem(SourceItem);
}

TMap<AHuntedInventoryItemBase*, FIntPoint> UPlayerInventoryComponent::GetAllItems()
{
	return GetAllItemsForGrid(EPlayerInventoryGridType::Inventory);
}

TMap<AHuntedInventoryItemBase*, FIntPoint> UPlayerInventoryComponent::GetAllItemsForGrid(
	EPlayerInventoryGridType GridType)
{
	CompactInvalidInventoryItems();
	TMap<AHuntedInventoryItemBase*, FIntPoint> GridItemMap;
	const TArray<AHuntedInventoryItemBase*>& GridItems = GetGridItems(GridType);
	
	for (int32 i = 0; i < GridItems.Num(); i++)
	{
		if (IsValid(GridItems[i]))
		{
			if (!GridItemMap.Contains(GridItems[i]))
			{
				GridItemMap.Add(GridItems[i], IndexToTile(static_cast<int8>(i)));
			}
		}
	}

	if (GridType == EPlayerInventoryGridType::Inventory)
	{
		AllItems = GridItemMap;
	}
	
	return GridItemMap;
}

bool UPlayerInventoryComponent::TryAddItem(AHuntedInventoryItemBase* ItemToAdd)
{
	CompactInvalidInventoryItems();

	if (!IsValid(ItemToAdd))
	{
		Debug::Print(TEXT("Player Inventory Component - Try To Add Item: No Item To Add reference on call"), FColor::Red);
		return false;
	}

	if (bOverflowResolutionActive)
	{
		return false;
	}

	const bool bIsVisibleWorldPickup = ItemToAdd->GetOwner() != GetOwner() && !ItemToAdd->IsHidden();

	const bool bCanUseStacking = ItemToAdd->IsItemStackable()
		&& ItemToAdd->GetItemInventorySize() == FIntPoint(1, 1)
		&& ItemToAdd->GetMaxStackPerCell() > 0;

	if (!bCanUseStacking)
	{
		for (int8 i = 0; i < Items.Num(); i++)
		{
			if (RoomForItemInInventory(ItemToAdd, i))
			{
				AHuntedInventoryItemBase* InventoryItem = ResolveInventoryStorageItem(ItemToAdd);
				if (!InventoryItem)
				{
					return false;
				}

				AddItemAtIndex(InventoryItem, i);
				return true;
			}
		}
	
		Debug::Print(TEXT("Player Inventory Component - Try To Add Item: No Room in Inventory"), FColor::Red);
		if (bIsVisibleWorldPickup)
		{
			TryBeginFullInventoryResolution(ItemToAdd);
		}
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
		if (!IsValid(ExistingStack) || !CanItemsStackTogether(ItemToAdd, ExistingStack))
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

		AHuntedInventoryItemBase* InventoryItem = ResolveInventoryStorageItem(ItemToAdd);
		if (!InventoryItem)
		{
			break;
		}

		InventoryItem->SetItemAmount(FMath::Min(RemainingAmount, MaxStackPerCell));
		AddItemAtIndex(InventoryItem, Index);
		RemainingAmount -= InventoryItem->GetItemAmount();
		break;
	}

	if (RemainingAmount > 0)
	{
		ItemToAdd->SetItemAmount(RemainingAmount);
		Debug::Print(TEXT("Player Inventory Component - Try To Add Item: Partial stack, no more room"), FColor::Yellow);
		RefreshInventoryGrid();
		if (bIsVisibleWorldPickup)
		{
			TryBeginFullInventoryResolution(ItemToAdd);
		}
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
		if (!IsValid(Item) || CountedItems.Contains(Item))
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

bool UPlayerInventoryComponent::CheckHaveItemByTag(FGameplayTag ItemTag) const
{
	if (!ItemTag.IsValid())
	{
		return 0;
	}
	
	TSet<const AHuntedInventoryItemBase*> CountedItems;

	for (AHuntedInventoryItemBase* Item : Items)
	{
		if (!IsValid(Item) || CountedItems.Contains(Item))
		{
			continue;
		}

		CountedItems.Add(Item);

		const FHuntedPlayerItemData ItemData = Item->GetItemData();
		if (ItemData.ItemTag == ItemTag)
		{
			return true;
		}

	}

	return false;
}

AHuntedInventoryItemBase* UPlayerInventoryComponent::GetItemByTag(FGameplayTag ItemTag) const
{
	if (!ItemTag.IsValid())
	{
		return nullptr;
	}
	
	TSet<const AHuntedInventoryItemBase*> CountedItems;

	for (AHuntedInventoryItemBase* Item : Items)
	{
		if (!IsValid(Item) || CountedItems.Contains(Item))
		{
			continue;
		}

		CountedItems.Add(Item);

		const FHuntedPlayerItemData ItemData = Item->GetItemData();
		if (ItemData.ItemTag == ItemTag)
		{
			return Item;
		}

	}

	return nullptr;
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
		if (!IsValid(Item) || ProcessedItems.Contains(Item))
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
			if (IsValid(Item))
			{
				Item->Destroy();
			}
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

bool UPlayerInventoryComponent::BeginCombineSelection(AHuntedInventoryItemBase* ItemToCombine)
{
	CompactInvalidInventoryItems();

	if (!IsValid(ItemToCombine) || !IsItemInInventory(ItemToCombine))
	{
		return false;
	}

	PendingCombineItem = ItemToCombine;
	RefreshInventoryGrid();
	return true;
}

void UPlayerInventoryComponent::CancelCombineSelection()
{
	if (!IsValid(PendingCombineItem))
	{
		PendingCombineItem = nullptr;
		return;
	}

	PendingCombineItem = nullptr;
	RefreshInventoryGrid();
}

bool UPlayerInventoryComponent::CanCombineItems(AHuntedInventoryItemBase* FirstItem, AHuntedInventoryItemBase* SecondItem) const
{
	FHuntedInventoryCombinationRecipe MatchingRecipe;
	int32 FirstAmount = 0;
	int32 SecondAmount = 0;
	return TryMatchCombinationRecipe(FirstItem, SecondItem, MatchingRecipe, FirstAmount, SecondAmount);
}

bool UPlayerInventoryComponent::TryCombineItems(AHuntedInventoryItemBase* FirstItem, AHuntedInventoryItemBase* SecondItem)
{
	CompactInvalidInventoryItems();

	FHuntedInventoryCombinationRecipe MatchingRecipe;
	int32 FirstAmount = 0;
	int32 SecondAmount = 0;
	if (!TryMatchCombinationRecipe(FirstItem, SecondItem, MatchingRecipe, FirstAmount, SecondAmount))
	{
		return false;
	}

	if (!MatchingRecipe.Result)
	{
		return false;
	}

	struct FConsumedItemSnapshot
	{
		AHuntedInventoryItemBase* Item = nullptr;
		int32 OriginalAmount = 0;
		bool bWasPresent = false;
		FIntPoint OriginalTopLeftTile = FIntPoint::ZeroValue;
	};

	TArray<FConsumedItemSnapshot> Snapshots;
	auto CaptureSnapshot = [this, &Snapshots](AHuntedInventoryItemBase* ItemToCapture)
	{
		if (!IsValid(ItemToCapture))
		{
			return;
		}

		for (const FConsumedItemSnapshot& ExistingSnapshot : Snapshots)
		{
			if (ExistingSnapshot.Item == ItemToCapture)
			{
				return;
			}
		}

		FConsumedItemSnapshot Snapshot;
		Snapshot.Item = ItemToCapture;
		Snapshot.OriginalAmount = ItemToCapture->GetItemAmount();
		Snapshot.bWasPresent = FindItemTopLeftTile(ItemToCapture, Snapshot.OriginalTopLeftTile);
		Snapshots.Add(Snapshot);
	};

	CaptureSnapshot(FirstItem);
	CaptureSnapshot(SecondItem);

	auto ConsumeSpecificItem = [this](AHuntedInventoryItemBase* ItemToConsume, int32 AmountToConsume) -> bool
	{
		if (!IsValid(ItemToConsume) || AmountToConsume <= 0)
		{
			return false;
		}

		if (ItemToConsume->IsItemStackable())
		{
			if (ItemToConsume->GetItemAmount() < AmountToConsume)
			{
				return false;
			}

			const int32 RemainingAmount = ItemToConsume->GetItemAmount() - AmountToConsume;
			ItemToConsume->SetItemAmount(RemainingAmount);
			if (RemainingAmount <= 0)
			{
				RemoveItem(ItemToConsume);
			}

			return true;
		}

		if (AmountToConsume > 1)
		{
			return false;
		}

		RemoveItem(ItemToConsume);
		return true;
	};

	const bool bUsingSingleStackForBothRequirements = FirstItem == SecondItem;
	if (bUsingSingleStackForBothRequirements)
	{
		if (!ConsumeSpecificItem(FirstItem, FirstAmount + SecondAmount))
		{
			return false;
		}
	}
	else
	{
		if (!ConsumeSpecificItem(FirstItem, FirstAmount) || !ConsumeSpecificItem(SecondItem, SecondAmount))
		{
			for (const FConsumedItemSnapshot& Snapshot : Snapshots)
			{
				if (!IsValid(Snapshot.Item))
				{
					continue;
				}

				Snapshot.Item->SetItemAmount(Snapshot.OriginalAmount);
				if (Snapshot.bWasPresent)
				{
					RemoveItem(Snapshot.Item);
					AddItemAtIndex(Snapshot.Item, TileToIndex(Snapshot.OriginalTopLeftTile));
				}
			}
			RefreshInventoryGrid();
			return false;
		}
	}

	AHuntedInventoryItemBase* ResultItem = SpawnInventoryItemInstance(MatchingRecipe.Result);
	if (!ResultItem)
	{
		for (const FConsumedItemSnapshot& Snapshot : Snapshots)
		{
			if (!IsValid(Snapshot.Item))
			{
				continue;
			}

			Snapshot.Item->SetItemAmount(Snapshot.OriginalAmount);
			if (Snapshot.bWasPresent)
			{
				RemoveItem(Snapshot.Item);
				AddItemAtIndex(Snapshot.Item, TileToIndex(Snapshot.OriginalTopLeftTile));
			}
		}
		RefreshInventoryGrid();
		return false;
	}

	ResultItem->SetItemAmount(FMath::Max(1, MatchingRecipe.ResultAmount));

	if (!TryAddItem(ResultItem))
	{
		if (IsValid(ResultItem))
		{
			ResultItem->Destroy();
		}

		for (const FConsumedItemSnapshot& Snapshot : Snapshots)
		{
			if (!IsValid(Snapshot.Item))
			{
				continue;
			}

			Snapshot.Item->SetItemAmount(Snapshot.OriginalAmount);
			if (Snapshot.bWasPresent)
			{
				RemoveItem(Snapshot.Item);
				AddItemAtIndex(Snapshot.Item, TileToIndex(Snapshot.OriginalTopLeftTile));
			}
		}

		RefreshInventoryGrid();
		return false;
	}

	for (const FConsumedItemSnapshot& Snapshot : Snapshots)
	{
		if (!IsValid(Snapshot.Item))
		{
			continue;
		}

		if (!IsItemInInventory(Snapshot.Item) && Snapshot.Item != ResultItem)
		{
			// Combination consumes hidden inventory actors; destroy only after all restoration paths are done.
			Snapshot.Item->Destroy();
		}
	}

	PendingCombineItem = nullptr;
	RefreshInventoryGrid();
	return true;
}

bool UPlayerInventoryComponent::CanItemCombineWithPendingSelection(AHuntedInventoryItemBase* CandidateItem) const
{
	if (!IsValid(PendingCombineItem) || !IsValid(CandidateItem))
	{
		return false;
	}

	return CanCombineItems(PendingCombineItem, CandidateItem);
}

bool UPlayerInventoryComponent::TryGetCombinationResultItemData(AHuntedInventoryItemBase* FirstItem,
	AHuntedInventoryItemBase* SecondItem, FHuntedPlayerItemData& OutResultItemData) const
{
	UMaterialInterface* ResultIcon = nullptr;
	return TryGetCombinationResultItemDisplayData(FirstItem, SecondItem, OutResultItemData, ResultIcon);
}

bool UPlayerInventoryComponent::TryGetCombinationResultItemDisplayData(AHuntedInventoryItemBase* FirstItem,
	AHuntedInventoryItemBase* SecondItem, FHuntedPlayerItemData& OutResultItemData,
	UMaterialInterface*& OutResultIcon) const
{
	FHuntedInventoryCombinationRecipe MatchingRecipe;
	int32 FirstAmount = 0;
	int32 SecondAmount = 0;
	OutResultIcon = nullptr;
	if (!TryMatchCombinationRecipe(FirstItem, SecondItem, MatchingRecipe, FirstAmount, SecondAmount)
		|| !MatchingRecipe.Result)
	{
		return false;
	}

	const AHuntedInventoryItemBase* ResultDefaultItem = MatchingRecipe.Result->GetDefaultObject<AHuntedInventoryItemBase>();
	if (!ResultDefaultItem)
	{
		return false;
	}

	OutResultItemData = ResultDefaultItem->GetItemData();
	OutResultIcon = ResultDefaultItem->GetIcon();
	return true;
}

bool UPlayerInventoryComponent::TryBeginFullInventoryResolution(AHuntedInventoryItemBase* PendingPickup)
{
	if (bOverflowResolutionActive)
	{
		return PendingOverflowPickup == PendingPickup;
	}

	if (!IsValid(PendingPickup) || IsItemInInventory(PendingPickup) || Items.Num() <= 0)
	{
		return false;
	}

	OverflowSnapshots.Reset();
	const TMap<AHuntedInventoryItemBase*, FIntPoint> CurrentItems = GetAllItems();
	OverflowSnapshots.Reserve(CurrentItems.Num());

	for (const TPair<AHuntedInventoryItemBase*, FIntPoint>& ItemPair : CurrentItems)
	{
		if (!IsValid(ItemPair.Key))
		{
			continue;
		}

		FInventoryOverflowItemSnapshot Snapshot;
		Snapshot.Item = ItemPair.Key;
		Snapshot.TopLeftTile = ItemPair.Value;
		Snapshot.ItemSize = ItemPair.Key->GetItemInventorySize();
		Snapshot.ItemAmount = ItemPair.Key->GetItemAmount();
		OverflowSnapshots.Add(Snapshot);
	}

	DiscardItems.Init(nullptr, Items.Num());

	int8 PendingPickupIndex = INDEX_NONE;
	for (int32 Index = 0; Index < DiscardItems.Num(); Index++)
	{
		if (RoomForItemInGridIgnoringItem(
			DiscardItems, PendingPickup, static_cast<int8>(Index), nullptr))
		{
			PendingPickupIndex = static_cast<int8>(Index);
			break;
		}
	}

	if (PendingPickupIndex == INDEX_NONE)
	{
		DiscardItems.Reset();
		OverflowSnapshots.Reset();
		return false;
	}

	PendingOverflowPickup = PendingPickup;
	PendingOverflowPickupOriginalSize = PendingPickup->GetItemInventorySize();
	PendingOverflowPickupOriginalAmount = PendingPickup->GetItemAmount();
	bOverflowResolutionActive = true;
	AddedItem = false;

	AddItemToGridAtIndex(DiscardItems, PendingPickup, PendingPickupIndex, false);
	RefreshInventoryGrids();
	OnInventoryOverflowRequested.Broadcast(PendingPickup);
	return true;
}

bool UPlayerInventoryComponent::ConfirmFullInventoryResolution()
{
	CompactInvalidInventoryItems();

	if (!bOverflowResolutionActive || !IsValid(PendingOverflowPickup))
	{
		return false;
	}

	const TMap<AHuntedInventoryItemBase*, FIntPoint> ItemsToDiscard =
		GetAllItemsForGrid(EPlayerInventoryGridType::Discard);

	for (const TPair<AHuntedInventoryItemBase*, FIntPoint>& ItemPair : ItemsToDiscard)
	{
		AHuntedInventoryItemBase* Item = ItemPair.Key;
		if (Item != PendingOverflowPickup && (!IsValid(Item) || !Item->IsItemDroppable()))
		{
			return false;
		}
	}

	FIntPoint PendingPickupTile;
	const bool bPickupAccepted = FindItemTopLeftTileInItems(
		Items, PendingOverflowPickup, PendingPickupTile);
	const bool bPickupRejected = FindItemTopLeftTileInItems(
		DiscardItems, PendingOverflowPickup, PendingPickupTile);

	if (!bPickupAccepted && !bPickupRejected)
	{
		return false;
	}

	for (const TPair<AHuntedInventoryItemBase*, FIntPoint>& ItemPair : ItemsToDiscard)
	{
		AHuntedInventoryItemBase* Item = ItemPair.Key;
		if (!IsValid(Item))
		{
			continue;
		}

		RemoveItemFromGrid(DiscardItems, Item);
		if (Item == PendingOverflowPickup)
		{
			Item->SetItemInventorySize(PendingOverflowPickupOriginalSize);
			Item->SetItemAmount(PendingOverflowPickupOriginalAmount);
			continue;
		}

		if (PendingCombineItem == Item)
		{
			PendingCombineItem = nullptr;
		}

		Item->Destroy();
	}

	if (bPickupAccepted)
	{
		PrepareItemForInventory(PendingOverflowPickup);
		AddedItem = true;
	}

	ResetOverflowResolutionState();
	RefreshInventoryGrids();
	OnInventoryOverflowClosed.Broadcast(bPickupAccepted);
	return true;
}

void UPlayerInventoryComponent::CancelFullInventoryResolution()
{
	if (!bOverflowResolutionActive)
	{
		return;
	}

	const int32 GridSize = FMath::Max<int32>(Items.Num(), Columns * Rows);
	Items.Init(nullptr, GridSize);

	for (const FInventoryOverflowItemSnapshot& Snapshot : OverflowSnapshots)
	{
		if (!IsValid(Snapshot.Item))
		{
			continue;
		}

		Snapshot.Item->SetItemInventorySize(Snapshot.ItemSize);
		Snapshot.Item->SetItemAmount(Snapshot.ItemAmount);
		AddItemToGridAtIndex(
			Items, Snapshot.Item, TileToIndex(Snapshot.TopLeftTile), true);
	}

	if (IsValid(PendingOverflowPickup))
	{
		PendingOverflowPickup->SetItemInventorySize(PendingOverflowPickupOriginalSize);
		PendingOverflowPickup->SetItemAmount(PendingOverflowPickupOriginalAmount);
	}

	ResetOverflowResolutionState();
	RefreshInventoryGrids();
	OnInventoryOverflowClosed.Broadcast(false);
}

void UPlayerInventoryComponent::ResetOverflowResolutionState()
{
	bOverflowResolutionActive = false;
	DiscardItems.Reset();
	OverflowSnapshots.Reset();
	PendingOverflowPickup = nullptr;
	PendingOverflowPickupOriginalSize = FIntPoint::ZeroValue;
	PendingOverflowPickupOriginalAmount = 0;
}

void UPlayerInventoryComponent::RequestDropItem(AHuntedInventoryItemBase* ItemToDrop)
{
	CompactInvalidInventoryItems();

	if (!IsValid(ItemToDrop) || !IsItemInInventory(ItemToDrop))
	{
		return;
	}

	OnItemDropRequested.Broadcast(ItemToDrop);
}

bool UPlayerInventoryComponent::DiscardItem(AHuntedInventoryItemBase* ItemToDiscard)
{
	CompactInvalidInventoryItems();

	if (!IsValid(ItemToDiscard) || !IsItemInInventory(ItemToDiscard))
	{
		return false;
	}

	if (!ItemToDiscard->IsItemDroppable())
	{
		return false;
	}

	if (PendingCombineItem == ItemToDiscard)
	{
		PendingCombineItem = nullptr;
	}

	RemoveItem(ItemToDiscard);
	if (IsValid(ItemToDiscard))
	{
		ItemToDiscard->Destroy();
	}
	RefreshInventoryGrid();
	return true;
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
	if (Items.IsValidIndex(Index) && IsValid(Items[Index]))
	{
		return Items[Index];
	}

	return nullptr;
}

bool UPlayerInventoryComponent::FindItemTopLeftTile(AHuntedInventoryItemBase* ItemToFind, FIntPoint& OutTopLeftTile) const
{
	return FindItemTopLeftTileInItems(Items, ItemToFind, OutTopLeftTile);
}

bool UPlayerInventoryComponent::FindItemTopLeftTileInGrid(AHuntedInventoryItemBase* ItemToFind,
	EPlayerInventoryGridType GridType, FIntPoint& OutTopLeftTile) const
{
	if (GridType == EPlayerInventoryGridType::Discard && !bOverflowResolutionActive)
	{
		return false;
	}

	return FindItemTopLeftTileInItems(GetGridItems(GridType), ItemToFind, OutTopLeftTile);
}

void UPlayerInventoryComponent::AddItemAtIndex(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex)
{
	if (!IsValid(ItemToAdd))
	{
		return;
	}

	const FIntPoint Dimensions = ItemToAdd->GetItemInventorySize();
	const FIntPoint Tile = IndexToTile(TopLeftIndex);

	if (Dimensions.X <= 0 || Dimensions.Y <= 0 || !GetResultAtIndex(TopLeftIndex))
	{
		return;
	}

	for (int16 i = Tile.X; i <= (Tile.X + Dimensions.X -1); i++)
	{
		for (int16 j = Tile.Y; j <= (Tile.Y + Dimensions.Y -1); j++)
		{
			if (!IsTileValid(FIntPoint(i, j)))
			{
				return;
			}
		}
	}

	PrepareItemForInventory(ItemToAdd);
	
	for (int16 i = Tile.X; i <= (Tile.X + Dimensions.X -1); i++)
	{
		for (int16 j = Tile.Y; j <= (Tile.Y + Dimensions.Y -1); j++)
		{
			const int8 ItemIndex = TileToIndex(FIntPoint(i, j));
			if (Items.IsValidIndex(ItemIndex))
			{
				Items[ItemIndex] = ItemToAdd;
			}
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
	if (!IsValid(SourceItem) || !IsValid(TargetItem) || SourceItem == TargetItem)
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
	if (!IsValid(SourceItem) || !GetResultAtIndex(TopLeftIndex))
	{
		return false;
	}

	AHuntedInventoryItemBase* TargetItem = Items[TopLeftIndex];
	if (!IsValid(TargetItem) || !CanItemsStackTogether(SourceItem, TargetItem))
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
	if (!IsValid(SourceItem) || !GetWorld())
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
	PrepareItemForInventory(NewStackItem);

	return NewStackItem;
}

AHuntedInventoryItemBase* UPlayerInventoryComponent::SpawnInventoryItemInstance(TSubclassOf<AHuntedInventoryItemBase> ItemClass) const
{
	if (!ItemClass || !GetWorld())
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AHuntedInventoryItemBase* SpawnedItem = GetWorld()->SpawnActor<AHuntedInventoryItemBase>(
		ItemClass,
		FVector(0.0f, 0.0f, -50000.0f),
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (SpawnedItem)
	{
		PrepareItemForInventory(SpawnedItem);
	}

	return SpawnedItem;
}

void UPlayerInventoryComponent::RefreshInventoryGrid() const
{
	RefreshInventoryGrids();
}

void UPlayerInventoryComponent::RefreshInventoryGrids() const
{
	if (IsValid(InventoryGridWidgetReference))
	{
		InventoryGridWidgetReference->RefreshItemWidgets();
	}

	if (IsValid(DiscardGridWidgetReference) && DiscardGridWidgetReference != InventoryGridWidgetReference)
	{
		DiscardGridWidgetReference->RefreshItemWidgets();
	}
}

void UPlayerInventoryComponent::SetInventoryGridWidget(UPlayerInventoryGridWidget* GridWidget,
	EPlayerInventoryGridType GridType)
{
	if (GridType == EPlayerInventoryGridType::Discard)
	{
		DiscardGridWidgetReference = GridWidget;
		return;
	}

	InventoryGridWidgetReference = GridWidget;
}

void UPlayerInventoryComponent::ClearInventoryGridWidget(UPlayerInventoryGridWidget* GridWidget,
	EPlayerInventoryGridType GridType)
{
	if (GridType == EPlayerInventoryGridType::Discard)
	{
		if (DiscardGridWidgetReference == GridWidget)
		{
			DiscardGridWidgetReference = nullptr;
		}
		return;
	}

	if (InventoryGridWidgetReference == GridWidget)
	{
		InventoryGridWidgetReference = nullptr;
	}
}

bool UPlayerInventoryComponent::IsItemInInventory(const AHuntedInventoryItemBase* Item) const
{
	if (!IsValid(Item))
	{
		return false;
	}

	for (AHuntedInventoryItemBase* InventoryItem : Items)
	{
		if (IsValid(InventoryItem) && InventoryItem == Item)
		{
			return true;
		}
	}

	return false;
}

bool UPlayerInventoryComponent::CanItemSatisfyAmount(const AHuntedInventoryItemBase* Item, int32 RequiredAmount) const
{
	if (!IsValid(Item) || RequiredAmount <= 0)
	{
		return false;
	}

	if (Item->IsItemStackable())
	{
		return Item->GetItemAmount() >= RequiredAmount;
	}

	return RequiredAmount == 1;
}

bool UPlayerInventoryComponent::TryMatchCombinationRecipe(const AHuntedInventoryItemBase* FirstItem,
	const AHuntedInventoryItemBase* SecondItem, FHuntedInventoryCombinationRecipe& OutRecipe, int32& OutFirstAmount,
	int32& OutSecondAmount) const
{
	OutFirstAmount = 0;
	OutSecondAmount = 0;

	if (!IsValid(FirstItem) || !IsValid(SecondItem) || !IsItemInInventory(FirstItem) || !IsItemInInventory(SecondItem))
	{
		return false;
	}

	for (const FHuntedInventoryCombinationRecipe& Recipe : CombinationRecipes)
	{
		if (!Recipe.Item1 || !Recipe.Item2 || !Recipe.Result)
		{
			continue;
		}

		const bool bDirectMatch = FirstItem->IsA(Recipe.Item1) && SecondItem->IsA(Recipe.Item2);
		const bool bReverseMatch = FirstItem->IsA(Recipe.Item2) && SecondItem->IsA(Recipe.Item1);
		if (!bDirectMatch && !bReverseMatch)
		{
			continue;
		}

		const int32 DirectFirstAmount = FMath::Max(1, Recipe.Amount1);
		const int32 DirectSecondAmount = FMath::Max(1, Recipe.Amount2);
		const int32 ReverseFirstAmount = FMath::Max(1, Recipe.Amount2);
		const int32 ReverseSecondAmount = FMath::Max(1, Recipe.Amount1);

		if (FirstItem == SecondItem)
		{
			if (!(Recipe.Item1 == Recipe.Item2))
			{
				continue;
			}

			const int32 TotalRequiredAmount = FMath::Max(1, Recipe.Amount1) + FMath::Max(1, Recipe.Amount2);
			if (!FirstItem->IsA(Recipe.Item1) || !CanItemSatisfyAmount(FirstItem, TotalRequiredAmount))
			{
				continue;
			}

			OutRecipe = Recipe;
			OutFirstAmount = FMath::Max(1, Recipe.Amount1);
			OutSecondAmount = FMath::Max(1, Recipe.Amount2);
			return true;
		}

		if (bDirectMatch && CanItemSatisfyAmount(FirstItem, DirectFirstAmount) && CanItemSatisfyAmount(SecondItem, DirectSecondAmount))
		{
			OutRecipe = Recipe;
			OutFirstAmount = DirectFirstAmount;
			OutSecondAmount = DirectSecondAmount;
			return true;
		}

		if (bReverseMatch && CanItemSatisfyAmount(FirstItem, ReverseFirstAmount) && CanItemSatisfyAmount(SecondItem, ReverseSecondAmount))
		{
			OutRecipe = Recipe;
			OutFirstAmount = ReverseFirstAmount;
			OutSecondAmount = ReverseSecondAmount;
			return true;
		}
	}

	return false;
}

bool UPlayerInventoryComponent::CanPlaceOrStackItemAtIndex(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex) const
{
	if (!IsValid(ItemToAdd))
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
	if (bOverflowResolutionActive)
	{
		return CanMoveItemToGridIndex(ItemToMove, EPlayerInventoryGridType::Inventory, TargetIndex);
	}

	if (!IsValid(ItemToMove) || !GetResultAtIndex(TargetIndex))
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
	if (!IsValid(TargetItem))
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
	if (bOverflowResolutionActive)
	{
		return TryMoveItemToGridIndex(
			ItemToMove, EPlayerInventoryGridType::Inventory, TargetIndex, bOutSourceConsumed);
	}

	bOutSourceConsumed = false;

	if (!IsValid(ItemToMove) || !GetResultAtIndex(TargetIndex))
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

bool UPlayerInventoryComponent::CanMoveItemToGridIndex(AHuntedInventoryItemBase* ItemToMove,
	EPlayerInventoryGridType TargetGridType, int8 TargetIndex) const
{
	if (!IsValid(ItemToMove))
	{
		return false;
	}

	if (!bOverflowResolutionActive)
	{
		return TargetGridType == EPlayerInventoryGridType::Inventory
			&& CanMoveItemToIndex(ItemToMove, TargetIndex);
	}

	if (TargetGridType == EPlayerInventoryGridType::Discard
		&& ItemToMove != PendingOverflowPickup
		&& !ItemToMove->IsItemDroppable())
	{
		return false;
	}

	EPlayerInventoryGridType SourceGridType;
	if (!FindItemGridType(ItemToMove, SourceGridType))
	{
		return false;
	}

	const TArray<AHuntedInventoryItemBase*>& TargetItems = GetGridItems(TargetGridType);
	return RoomForItemInGridIgnoringItem(TargetItems, ItemToMove, TargetIndex,
		SourceGridType == TargetGridType ? ItemToMove : nullptr);
}

bool UPlayerInventoryComponent::TryMoveItemToGridIndex(AHuntedInventoryItemBase* ItemToMove,
	EPlayerInventoryGridType TargetGridType, int32 TargetIndex, bool& bOutSourceConsumed)
{
	bOutSourceConsumed = false;

	if (!bOverflowResolutionActive)
	{
		return TargetGridType == EPlayerInventoryGridType::Inventory
			&& TryMoveItemToIndex(ItemToMove, TargetIndex, bOutSourceConsumed);
	}

	if (!CanMoveItemToGridIndex(ItemToMove, TargetGridType, static_cast<int8>(TargetIndex)))
	{
		return false;
	}

	EPlayerInventoryGridType SourceGridType;
	if (!FindItemGridType(ItemToMove, SourceGridType))
	{
		return false;
	}

	TArray<AHuntedInventoryItemBase*>& SourceItems = GetMutableGridItems(SourceGridType);
	TArray<AHuntedInventoryItemBase*>& TargetItems = GetMutableGridItems(TargetGridType);
	RemoveItemFromGrid(SourceItems, ItemToMove);
	AddItemToGridAtIndex(TargetItems, ItemToMove, static_cast<int8>(TargetIndex), false);
	RefreshInventoryGrids();
	return true;
}
