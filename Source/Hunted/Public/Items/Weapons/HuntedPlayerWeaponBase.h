// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/HuntedWeaponBase.h"
#include "HuntedTypes/HuntedStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
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
	
	UFUNCTION(BlueprintCallable)
	void AssignGratedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);
	
	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandle() const;
	
private:
	TArray<FGameplayAbilitySpecHandle>	GrantedAbilitySpecHandles;
};

