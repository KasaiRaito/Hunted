// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInventoryComponent.generated.h"


class AHutedInventoryItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HUNTED_API UPlayerInventoryComponent : public UActorComponent
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
	TArray<AHutedInventoryItemBase*> Items;
	
	bool IsTileValid(FIntPoint Tile);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(AHutedInventoryItemBase* ItemToAdd);
	
	UFUNCTION()
	bool RoomForItemInInventory(AHutedInventoryItemBase* ItemToAdd, int8 TopLeftIndex);
		
	UFUNCTION()
	FIntPoint IndexToTile(int8 Index);
	
	UFUNCTION()
	int8 TileToIndex(FIntPoint Tile);
	
	UFUNCTION()
	bool GetResultAtIndex(int8 Index);
	
	UFUNCTION()
	AHutedInventoryItemBase* GetItemAtIndex(int8 Index);
	
	UFUNCTION()
	void AddItemAtIndex(AHutedInventoryItemBase* ItemToAdd, int8 TopLeftIndex);
};
