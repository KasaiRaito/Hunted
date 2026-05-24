// KasaiRaito All Rights Reserved


#include "AbilitySystem/Abilities/HuntedPlayerGameplayAbility.h"
//#include "Components/Combat/PlayerCombatComponent.h"
//#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Characters/HuntedPlayerCharacter.h"
#include "Controllers/HuntedPlayerController.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "HuntedGameplayTags.h"

AHuntedPlayerCharacter* UHuntedPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	if (!CachedWarriorHeroCharacter.IsValid())
	{
		CachedWarriorHeroCharacter = CurrentActorInfo
			? Cast<AHuntedPlayerCharacter>(CurrentActorInfo->AvatarActor.Get())
			: nullptr;
	}

	return CachedWarriorHeroCharacter.IsValid() ? CachedWarriorHeroCharacter.Get() : nullptr;
}


AHuntedPlayerController* UHuntedPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!CachedWarriorHeroController.IsValid())
	{
		CachedWarriorHeroController = CurrentActorInfo
			? Cast<AHuntedPlayerController>(CurrentActorInfo->PlayerController.Get())
			: nullptr;
	}

	return CachedWarriorHeroController.IsValid() ? CachedWarriorHeroController.Get() : nullptr;
}

UPlayerCombatComponent* UHuntedPlayerGameplayAbility::GetPlayerCombatComponentForActorInfo()
{
	AHuntedPlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	return IsValid(PlayerCharacter) ? PlayerCharacter->GetPlayerCombatComponent() : nullptr;
}

UPlayerInventoryComponent* UHuntedPlayerGameplayAbility::GetPlayerInventoryComponentForActorInfo()
{
	AHuntedPlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	return IsValid(PlayerCharacter) ? PlayerCharacter->GetPlayerInventoryComponent() : nullptr;
}

FGameplayEffectSpecHandle UHuntedPlayerGameplayAbility::MakePlayerSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,
                                                                             float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, float DamageScalar)
{
	if (!EffectClass)
	{
		return FGameplayEffectSpecHandle();
	}

	UHuntedAbilitySystemComponent* ASC = GetHuntedAbilitySystemComponentFromActorInfo();
	AActor* AvatarActor = CurrentActorInfo ? CurrentActorInfo->AvatarActor.Get() : nullptr;
	if (!ASC || !IsValid(AvatarActor))
	{
		// Damage specs can be requested during ability cancellation; invalid context should fail quietly.
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
		InWeaponBaseDamage
	);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(
			InCurrentAttackTypeTag, DamageScalar
		);
	}
	
	return EffectSpecHandle;
}

