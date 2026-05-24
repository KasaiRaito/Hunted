// KasaiRaito All Rights Reserved


#include "AbilitySystem/Abilities/HuntedGameplayAbility.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UHuntedGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EHuntedAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid() && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UHuntedGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EHuntedAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UHuntedGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	AActor* AvatarActor = CurrentActorInfo ? CurrentActorInfo->AvatarActor.Get() : nullptr;
	return IsValid(AvatarActor) ? AvatarActor->FindComponentByClass<UPawnCombatComponent>() : nullptr;
}

UHuntedAbilitySystemComponent* UHuntedGameplayAbility::GetHuntedAbilitySystemComponentFromActorInfo() const
{
	return CurrentActorInfo
		? Cast<UHuntedAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get())
		: nullptr;
}

FActiveGameplayEffectHandle UHuntedGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	if (!TargetASC || !InSpecHandle.IsValid() || !InSpecHandle.Data.IsValid())
	{
		// Effects can be requested by Blueprint after a target dies; return a failed handle instead of crashing.
		return FActiveGameplayEffectHandle();
	}

	UHuntedAbilitySystemComponent* SourceASC = GetHuntedAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
	{
		return FActiveGameplayEffectHandle();
	}
	
	return SourceASC->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,
		TargetASC
	);
}

FActiveGameplayEffectHandle UHuntedGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle, EHuntedSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);
	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? 
		EHuntedSuccessType::Successful : EHuntedSuccessType::Failed;
	
	return ActiveGameplayEffectHandle;
}
