// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HuntedPlayerGameplayAbility.h"
#include "PlayerGameplayAbility_UseItems.generated.h"

/**
 * 
 */
UCLASS()
class HUNTED_API UPlayerGameplayAbility_UseItems : public UHuntedPlayerGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ Ebd UGameplayAbility Interface
	
	UFUNCTION(BlueprintCallable, Category = "Hunted|AbilitySystem|UseItems")
	void UseItem();
	
};
