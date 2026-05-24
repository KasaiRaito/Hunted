// KasaiRaito All Rights Reserved


#include "AbilitySystem/Abilities/HuntedEnemyGameplayAbility.h"

#include "Characters/HuntedEnemyCharacter.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "HuntedGameplayTags.h"

AHuntedEnemyCharacter* UHuntedEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedHuntedEnemyCharacter.IsValid())
	{
		CachedHuntedEnemyCharacter = CurrentActorInfo
			? Cast<AHuntedEnemyCharacter>(CurrentActorInfo->AvatarActor.Get())
			: nullptr;
	}
	return CachedHuntedEnemyCharacter.IsValid()? CachedHuntedEnemyCharacter.Get(): nullptr;
}

UEnemyCombatComponent* UHuntedEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	AHuntedEnemyCharacter* EnemyCharacter = GetEnemyCharacterFromActorInfo();
	return IsValid(EnemyCharacter) ? EnemyCharacter->GetEnemyCombatComponent() : nullptr;
}

FGameplayEffectSpecHandle UHuntedEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
	if (!EffectClass)
	{
		return FGameplayEffectSpecHandle();
	}

	UHuntedAbilitySystemComponent* ASC = GetHuntedAbilitySystemComponentFromActorInfo();
	AActor* AvatarActor = CurrentActorInfo ? CurrentActorInfo->AvatarActor.Get() : nullptr;
	if (!ASC || !IsValid(AvatarActor))
	{
		// Enemy effects can be queued while the enemy is being destroyed.
		return FGameplayEffectSpecHandle();
	}
	
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(AvatarActor);
	ContextHandle.AddInstigator(AvatarActor, AvatarActor);
	
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	if (!EffectSpecHandle.Data.IsValid())
	{
		return FGameplayEffectSpecHandle();
	}
	
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
	HuntedGameplayTags::Shared_SetByCaller_BaseDamage,
	InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
	);
	
	return EffectSpecHandle;
}
