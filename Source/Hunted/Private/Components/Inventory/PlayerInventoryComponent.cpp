// KasaiRaito All Rights Reserved


#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"

#include "HuntedDebugHelper.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

bool UPlayerInventoryComponent::IsTileValid(FIntPoint Tile)
{
	if (Tile.X >= 0 || Tile.Y >= 0  && Tile.X < Columns && Tile.Y <= Rows)
	{
		return true;
	}
	
	return false;
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

bool UPlayerInventoryComponent::TryAddItem(AHuntedInventoryItemBase* ItemToAdd)
{
	if (!ItemToAdd)
	{
		Debug::Print(TEXT("Player Inventory Component - Try To Add Item: No Item To Add reference on call"), FColor::Red);
		return false;
	}
	
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

bool UPlayerInventoryComponent::RoomForItemInInventory(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex)
{
	FIntPoint Dimensions = ItemToAdd->GetItemInventorySize();
	FIntPoint Tile = IndexToTile(TopLeftIndex);
	
	for (int8 i = Tile.X; i <= (Tile.X + Dimensions.X -1); i++)
	{
		for (int8 j = Tile.Y; j <= (Tile.Y + Dimensions.Y -1); j++)
		{
			if (IsTileValid(FIntPoint(i, j)))
			{
				int8 Index = TileToIndex(FIntPoint(i, j));
				
				if (GetResultAtIndex(Index))
				{
					if (GetItemAtIndex(Index))
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	
	return true;
}

FIntPoint UPlayerInventoryComponent::IndexToTile(int8 Index)
{
	return FIntPoint(Index % Columns, Index / Columns);
}

int8 UPlayerInventoryComponent::TileToIndex(FIntPoint Tile)
{
	int8 Index = Tile.X + Tile.Y * Columns;
	return Index;
}

bool UPlayerInventoryComponent::GetResultAtIndex(int8 Index)
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

void UPlayerInventoryComponent::AddItemAtIndex(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex)
{
	FIntPoint Dimensions = ItemToAdd->GetItemInventorySize();
	FIntPoint Tile = IndexToTile(TopLeftIndex);
	
	for (int8 i = Tile.X; i <= (Tile.X + Dimensions.X -1); i++)
	{
		for (int8 j = Tile.Y; j <= (Tile.Y + Dimensions.Y -1); j++)
		{
			Items[TileToIndex(FIntPoint(i, j))] = ItemToAdd;
		}
	}
	
}