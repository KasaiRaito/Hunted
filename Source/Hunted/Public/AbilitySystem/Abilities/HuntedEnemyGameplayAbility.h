// KasaiRaito All Rights Reserved

#pragma once

#include "AbilitySystem/Abilities/HuntedGameplayAbility.h"
#include "HuntedEnemyGameplayAbility.generated.h"

class AHuntedEnemyCharacter;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedEnemyGameplayAbility : public UHuntedGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AHuntedEnemyCharacter* GetEnemyCharacterFromActorInfo();
	
	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();
	
private:
	TWeakObjectPtr<AHuntedEnemyCharacter> CachedHuntedEnemyCharacter;
};
