// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Components/PawnExtensionComponentBase.h"
#include "HuntedTypes/HuntedStructTypes.h"
#include "PlayerInventoryComponent.generated.h"


class UPlayerInventoryGridWidget;
class AHuntedInventoryItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HUNTED_API UPlayerInventoryComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Transient)
	TMap<AHuntedInventoryItemBase*, FIntPoint> AllItems;
	
	UPlayerInventoryComponent();
	
	UPROPERTY(EditAnywhere, Category= "InventoryComponent Info | Inventory Size")
	int8 Columns;
	UPROPERTY(EditAnywhere, Category= "InventoryComponent Info | Inventory Size")
	int8 Rows;
	
	UPROPERTY(EditAnywhere, Category= "InventoryComponent Info | Inventory Size")
	float TileSize;
	
	UPROPERTY(Transient)
	TArray<AHuntedInventoryItemBase*> Items;
	
	UPROPERTY(EditAnywhere, Category= "InventoryComponent Info | Item Counters")
	int16 BaseBulletsCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryComponent | Combination")
	TArray<FHuntedInventoryCombinationRecipe> CombinationRecipes;
	
	bool AddedItem = false;

	UPROPERTY(Transient)
	AHuntedInventoryItemBase* PendingCombineItem = nullptr;
	
	UPROPERTY(Transient)
	UPlayerInventoryGridWidget* InventoryGridWidgetReference = nullptr;
	
	bool IsTileValid(FIntPoint Tile) const;
	bool RoomForItemInInventoryIgnoringItem(const AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex,
		const AHuntedInventoryItemBase* IgnoredItem) const;
	void CompactInvalidInventoryItems();
	void PrepareItemForInventory(AHuntedInventoryItemBase* ItemToPrepare) const;
	AHuntedInventoryItemBase* ResolveInventoryStorageItem(AHuntedInventoryItemBase* SourceItem);
	bool CanItemsStackTogether(const AHuntedInventoryItemBase* SourceItem, const AHuntedInventoryItemBase* TargetItem) const;
	bool CanStackItemAtIndex(const AHuntedInventoryItemBase* SourceItem, int8 TopLeftIndex) const;
	AHuntedInventoryItemBase* SpawnStackCloneFromItem(const AHuntedInventoryItemBase* SourceItem) const;
	AHuntedInventoryItemBase* SpawnInventoryItemInstance(TSubclassOf<AHuntedInventoryItemBase> ItemClass) const;
	void RefreshInventoryGrid() const;
	bool IsItemInInventory(const AHuntedInventoryItemBase* Item) const;
	bool CanItemSatisfyAmount(const AHuntedInventoryItemBase* Item, int32 RequiredAmount) const;
	bool TryMatchCombinationRecipe(const AHuntedInventoryItemBase* FirstItem, const AHuntedInventoryItemBase* SecondItem,
		FHuntedInventoryCombinationRecipe& OutRecipe, int32& OutFirstAmount, int32& OutSecondAmount) const;
	
public:
	UFUNCTION()	
	FORCEINLINE int8 GetColumns() { return Columns; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory-Component", meta = (DisplayName = "GetColumns"))
	FORCEINLINE int BP_GetColumns() { return Columns; }
	
	UFUNCTION()	
	FORCEINLINE int8 GetRows() { return Rows; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory-Component", meta = (DisplayName = "GetRows"))
	FORCEINLINE int BP_GetRows() { return GetRows(); }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory-Component")	
	FORCEINLINE float GetTileSize() { return TileSize; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory-Component")
	FORCEINLINE TArray<AHuntedInventoryItemBase*> GetItems() { return Items; }
	
	UFUNCTION()
	FORCEINLINE void SetItemsNum(int8 size) { Items.SetNum(size); }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory-Component", meta = (DisplayName = "GetRows"))
	FORCEINLINE void PB_SetItemsNum(int size) { SetItemsNum(size); }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory-Component")
	FORCEINLINE UPlayerInventoryGridWidget* GetPlayerInventoryGridWidget() { return InventoryGridWidgetReference; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool GetAddedItem() { return AddedItem; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetAddedItem(bool val) { AddedItem = val; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE int GetBaseBulletsCount () { return BaseBulletsCount; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void SetBaseBulletsCount (int val) { BaseBulletsCount = val; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void AddToBaseBulletsCount(int val) { BaseBulletsCount += val; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void SubtractToBaseBulletsCount(int val) { BaseBulletsCount -= val; }
	
	
protected:
	
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TMap<AHuntedInventoryItemBase*, FIntPoint> GetAllItems();
	
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(AHuntedInventoryItemBase* ItemToAdd);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetTotalItemAmountByTag(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (DisplayName = "Get Available Item Amount By Tag"))
	int32 GetAvailableItemAmountByTag(FGameplayTag ItemTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (DispayName = "CheckHaveItemByTag"))
	bool CheckHaveItemByTag(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItemAmountByTag(FGameplayTag ItemTag, int32 RequiredAmount) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryRemoveItemAmountByTag(FGameplayTag ItemTag, int32 AmountToRemove);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Combination")
	bool BeginCombineSelection(AHuntedInventoryItemBase* ItemToCombine);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Combination")
	void CancelCombineSelection();

	UFUNCTION(BlueprintPure, Category = "Inventory|Combination")
	bool IsCombineModeActive() const { return PendingCombineItem != nullptr; }

	UFUNCTION(BlueprintPure, Category = "Inventory|Combination")
	AHuntedInventoryItemBase* GetPendingCombineItem() const { return PendingCombineItem; }

	UFUNCTION(BlueprintCallable, Category = "Inventory|Combination")
	bool CanCombineItems(AHuntedInventoryItemBase* FirstItem, AHuntedInventoryItemBase* SecondItem) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Combination")
	bool TryCombineItems(AHuntedInventoryItemBase* FirstItem, AHuntedInventoryItemBase* SecondItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Combination")
	bool CanItemCombineWithPendingSelection(AHuntedInventoryItemBase* CandidateItem) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Combination")
	bool TryGetCombinationResultItemData(AHuntedInventoryItemBase* FirstItem, AHuntedInventoryItemBase* SecondItem,
		FHuntedPlayerItemData& OutResultItemData) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DiscardItem(AHuntedInventoryItemBase* ItemToDiscard);
	
	UFUNCTION()
	bool RoomForItemInInventory(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex) const;
		
	UFUNCTION()
	FIntPoint IndexToTile(int8 Index) const;
	
	UFUNCTION()
	int8 TileToIndex(FIntPoint Tile) const;
	
	UFUNCTION()
	bool GetResultAtIndex(int8 Index) const;
	
	UFUNCTION()
	AHuntedInventoryItemBase* GetItemAtIndex(int8 Index);

	UFUNCTION()
	bool FindItemTopLeftTile(AHuntedInventoryItemBase* ItemToFind, FIntPoint& OutTopLeftTile) const;
	
	UFUNCTION()
	void AddItemAtIndex(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex);

	UFUNCTION()
	bool CanPlaceOrStackItemAtIndex(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex) const;

	UFUNCTION()
	bool CanMoveItemToIndex(AHuntedInventoryItemBase* ItemToMove, int8 TargetIndex) const;

	UFUNCTION()
	bool TryStackItemAtIndex(AHuntedInventoryItemBase* ItemToStack, int8 TopLeftIndex, bool& bOutSourceConsumed);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryMoveItemToIndex(AHuntedInventoryItemBase* ItemToMove, int TargetIndex, bool& bOutSourceConsumed);
	
	UFUNCTION(BlueprintCallable)
	void SetInventoryGridWidget(UPlayerInventoryGridWidget* GridWidget) { InventoryGridWidgetReference = GridWidget; };
	
	void RemoveItem(AHuntedInventoryItemBase* ItemToRemove);
};
