// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/HuntedWeaponBase.h"
#include "HuntedTypes/HuntedStructTypes.h"
#include "HuntedPlayerWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class HUNTED_API AHuntedPlayerWeaponBase : public AHuntedWeaponBase
{
	GENERATED_BODY()

public:
	//MIGHT HAVE TO CHANGE LOGIC TO A CHILD CLASS SPECIFIC WEAPON
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FHuntedPlayerWeaponData WeaponData;
};

