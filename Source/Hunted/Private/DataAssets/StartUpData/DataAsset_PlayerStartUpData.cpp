// KasaiRaito All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_PlayerStartUpData.h"
#include "AbilitySystem/Abilities/HuntedGameplayAbility.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"

void UDataAsset_PlayerStartUpData::GivenToAbilitySystemComponent(UHuntedAbilitySystemComponent* InASCToGive,
	int32 ApplyLevel)
{
	Super::GivenToAbilitySystemComponent(InASCToGive, ApplyLevel);

	for (const FHuntedPlayerAbilitySet& AbilitySet : PlayerStartUpAbilitySets)
	{
		if (!AbilitySet.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
