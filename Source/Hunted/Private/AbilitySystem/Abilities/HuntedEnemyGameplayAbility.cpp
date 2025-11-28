// KasaiRaito All Rights Reserved


#include "AbilitySystem/Abilities/HuntedEnemyGameplayAbility.h"

#include "Characters/HuntedEnemyCharacter.h"

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
