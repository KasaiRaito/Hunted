// KasaiRaito All Rights Reserved

#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/HuntedEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GivenToAbilitySystemComponent(UHuntedAbilitySystemComponent* InASCToGive,
	int32 ApplyLevel)
{
	Super::GivenToAbilitySystemComponent(InASCToGive, ApplyLevel);
	
	if (!EnemyCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf<UHuntedEnemyGameplayAbility> & AbilityClass : EnemyCombatAbilities)
		{
			if (!AbilityClass)
			{
				continue;
			}
			
			FGameplayAbilitySpec AbilitySpec;
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;
			
			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
