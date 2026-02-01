// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HuntedGameplayAbility.h"
#include "HuntedPlayerGameplayAbility.generated.h"

class AHuntedPlayerCharacter;
class AHuntedPlayerController;
/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedPlayerGameplayAbility : public UHuntedGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Player|Ability")
	AHuntedPlayerCharacter* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Player|Ability")
	AHuntedPlayerController* GetPlayerControllerFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Player|Ability")
	UPlayerCombatComponent* GetPlayerCombatComponentForActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Player|Ability")
	FGameplayEffectSpecHandle MakePlayerSpecHandle
		(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, 
			FGameplayTag InCurrentAttackTypeTag, float DamageScalar);
	
private:
	TWeakObjectPtr<AHuntedPlayerCharacter> CachedWarriorHeroCharacter;
	TWeakObjectPtr<AHuntedPlayerController> CachedWarriorHeroController;
};
