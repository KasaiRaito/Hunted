// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

/**
 * 
 */

class AHuntedWeaponBase;
UCLASS()
class HUNTED_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Warriro|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AHuntedWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Warriro|Combat")
	AHuntedWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "Warriro|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Warriro|Combat")
	AHuntedWeaponBase* GetCharacterCurrentEquippedWeapon() const;
	
private:
	TMap<FGameplayTag, AHuntedWeaponBase*> CharacterCarriedWeaponMap;
};
