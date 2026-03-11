// KasaiRaito All Rights Reserved


#include "AbilitySystem/Abilities/HuntedEnemyGameplayAbility.h"

#include "Characters/HuntedEnemyCharacter.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "HuntedGameplayTags.h"

AHuntedEnemyCharacter* UHuntedEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedHuntedEnemyCharacter.IsValid())
	{
		CachedHuntedEnemyCharacter = Cast<AHuntedEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedHuntedEnemyCharacter.IsValid()? CachedHuntedEnemyCharacter.Get(): nullptr;
}

UEnemyCombatComponent* UHuntedEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UHuntedEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
	check(EffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetHuntedAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(),GetAvatarActorFromActorInfo());
	
	FGameplayEffectSpecHandle EffectSpecHandle = GetHuntedAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);
	
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
	HuntedGameplayTags::Shared_SetByCaller_BaseDamage,
	InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
	);
	
	return EffectSpecHandle;
}
