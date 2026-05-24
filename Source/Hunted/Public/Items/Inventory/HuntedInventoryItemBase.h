// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "HuntedInteractable.h"
#include "GameFramework/Actor.h"
#include "HuntedTypes/HuntedStructTypes.h"
#include "HuntedInventoryItemBase.generated.h"

class USphereComponent;
class UWidgetComponent;

UCLASS()
class HUNTED_API AHuntedInventoryItemBase : public AHuntedInteractable
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Item-World")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item-World")
	USphereComponent* CollisionSphere;
	
	UPROPERTY(EditAnywhere, Category = "Item-World")
	bool  ItemIsInVew;
	
	UPROPERTY(EditAnywhere, Category = "Item-World")
	bool IsInRangeOfPlayer;	
	
	UPROPERTY(EditAnywhere, Category = "Item-Inventory | Info")
	FIntPoint ObjectInventorySize;
	
	UPROPERTY(EditAnywhere, Category = "Item-Inventory | Info")
	UMaterialInterface* Icon;
	
	UPROPERTY(EditAnywhere, Category = "Item-Inventory | Info")
	FHuntedPlayerItemData ItemData;
	
protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* InteractWidgetComponent;
	
public:
	AHuntedInventoryItemBase();
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE FIntPoint GetItemInventorySize() const { return ObjectInventorySize; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE void SetItemInventorySize(FIntPoint NewSize) { ObjectInventorySize = NewSize; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE bool GetItemIsInVew() { return ItemIsInVew;};
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	void SetItemIsInVew(bool val);
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE bool GetIsInRangeOfPlayer() { return IsInRangeOfPlayer;};
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE void SetIsInRangeOfPlayer(bool val) { IsInRangeOfPlayer = val; };
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE UMaterialInterface* GetIcon() const { return Icon; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE void SetIcon(UMaterialInterface* NewIcon) { Icon = NewIcon; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE FHuntedPlayerItemData GetItemData() const { return ItemData; }

	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE void SetItemData(const FHuntedPlayerItemData& NewItemData) { ItemData = NewItemData; }

	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE int32 GetItemAmount() const { return ItemData.ItemAmount; }

	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE void SetItemAmount(int32 NewAmount) { ItemData.ItemAmount = FMath::Max(0, NewAmount); }

	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE void AddItemAmount(int32 DeltaAmount) { SetItemAmount(ItemData.ItemAmount + DeltaAmount); }

	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE bool IsItemStackable() const { return ItemData.IsStackable; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE bool IsItemDroppable() const { return ItemData.IsDroppable; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE int32 GetMaxStackPerCell() const { return ItemData.MaxStackPerCell; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
