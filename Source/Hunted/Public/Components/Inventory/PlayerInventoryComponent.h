// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PlayerInventoryComponent.generated.h"


class AHuntedInventoryItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HUNTED_API UPlayerInventoryComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:	
	UPlayerInventoryComponent();
	
	UPROPERTY(EditAnywhere, Category= "InventoryComponent Info | Inventory Size")
	int8 Columns;
	UPROPERTY(EditAnywhere, Category= "InventoryComponent Info | Inventory Size")
	int8 Rows;
	
	UPROPERTY(EditAnywhere, Category= "InventoryComponent Info | Inventory Size")
	float TileSize;
	
	UPROPERTY(EditDefaultsOnly, Category= "InventoryComponent Info | Inventory Size")
	TArray<AHuntedInventoryItemBase*> Items;
	
	bool IsTileValid(FIntPoint Tile);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(AHuntedInventoryItemBase* ItemToAdd);
	
	UFUNCTION()
	bool RoomForItemInInventory(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex);
		
	UFUNCTION()
	FIntPoint IndexToTile(int8 Index);
	
	UFUNCTION()
	int8 TileToIndex(FIntPoint Tile);
	
	UFUNCTION()
	bool GetResultAtIndex(int8 Index);
	
	UFUNCTION()
	AHuntedInventoryItemBase* GetItemAtIndex(int8 Index);
	
	UFUNCTION()
	void AddItemAtIndex(AHuntedInventoryItemBase* ItemToAdd, int8 TopLeftIndex);
};
