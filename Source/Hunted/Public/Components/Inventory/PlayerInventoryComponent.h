// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInventoryComponent.generated.h"


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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
