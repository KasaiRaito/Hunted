// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HutedInventoryItemBase.generated.h"

class USphereComponent;

UCLASS()
class HUNTED_API AHutedInventoryItemBase : public AActor
{
	GENERATED_BODY()
	
public:
	AHutedInventoryItemBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "Item-World")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item-World")
	USphereComponent* CollisionSphere;

};
