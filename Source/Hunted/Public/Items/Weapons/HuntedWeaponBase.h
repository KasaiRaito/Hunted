// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HuntedWeaponBase.generated.h"

class UBoxComponent;

UCLASS()
class HUNTED_API AHuntedWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHuntedWeaponBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons-Knifes")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons-Knifes")
	UBoxComponent* WeaponCollisionBox;

public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox;}
};
