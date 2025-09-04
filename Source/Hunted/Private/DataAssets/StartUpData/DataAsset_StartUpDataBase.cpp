// KasaiRaito All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/HuntedGameplayAbility.h"

void UDataAsset_StartUpDataBase::GivenToAbilitySystemComponent(UHuntedAbilitySystemComponent* InASCToGive,
	int32 ApplyLevel)
{
	check(InASCToGive);

	GranAbilities(ActivateOnGivenAbilities,InASCToGive,ApplyLevel);
	GranAbilities(ReactiveAbilities,InASCToGive,ApplyLevel);	
}

void UDataAsset_StartUpDataBase::GranAbilities(const TArray<TSubclassOf<UHuntedGameplayAbility>>& InAbilitiesToGive,
	UHuntedAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UHuntedGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		
		InASCToGive->GiveAbility(AbilitySpec);
	}
}
