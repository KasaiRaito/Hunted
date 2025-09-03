// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HuntedKnifeBase.generated.h"

class UBoxComponent;

UCLASS()
class HUNTED_API AHuntedKnifeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHuntedKnifeBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons-Knifes")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons-Knifes")
	UBoxComponent* WeaponCollisionBox;

public:
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox;}
};
