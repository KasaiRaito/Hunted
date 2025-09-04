// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "HuntedGameplayAbility.generated.h"

/**
 * 
 */

class UPawnCombatComponent;

UENUM(BlueprintType)
enum class EHuntedAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven,
};

UCLASS()
class HUNTED_API UHuntedGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplay Interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplay Interface
	
	UPROPERTY(EditDefaultsOnly, Category= "HuntedAbility")
	EHuntedAbilityActivationPolicy AbilityActivationPolicy = EHuntedAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category= "Hunted|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;
	
};
