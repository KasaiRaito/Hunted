// KasaiRaito All Rights Reserved


#include "AbilitySystem/Abilities/HuntedPlayerGameplayAbility.h"
#include "Characters/HuntedPlayerCharacter.h"
#include "Controllers/HuntedPlayerController.h"

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