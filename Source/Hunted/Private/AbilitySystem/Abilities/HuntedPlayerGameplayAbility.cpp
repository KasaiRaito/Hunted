// KasaiRaito All Rights Reserved


#include "AbilitySystem/Abilities/HuntedPlayerGameplayAbility.h"
#include "Characters/HuntedPlayerCharacter.h"
#include "Controllers/HuntedPlayerController.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "HuntedGameplayTags.h"

AHuntedPlayerCharacter* UHuntedPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	if (!CachedWarriorHeroCharacter.IsValid())
	{
		CachedWarriorHeroCharacter = Cast<AHuntedPlayerCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedWarriorHeroCharacter.IsValid() ? CachedWarriorHeroCharacter.Get() : nullptr;
}


AHuntedPlayerController* UHuntedPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!CachedWarriorHeroController.IsValid())
	{
		CachedWarriorHeroController = Cast<AHuntedPlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedWarriorHeroController.IsValid() ? CachedWarriorHeroController.Get() : nullptr;
}

UPlayerCombatComponent* UHuntedPlayerGameplayAbility::GetPlayerCombatComponentForActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}

FGameplayEffectSpecHandle UHuntedPlayerGameplayAbility::MakePlayerSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,
	float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
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
		InWeaponBaseDamage
	);
	
	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(
			InCurrentAttackTypeTag, InUsedComboCount
		);
	}
	
	return EffectSpecHandle;
}

