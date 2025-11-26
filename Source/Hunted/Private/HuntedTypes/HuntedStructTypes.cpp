// KasaiRaito All Rights Reserved

#include "HuntedTypes/HuntedStructTypes.h"
#include "AbilitySystem/Abilities/HuntedGameplayAbility.h"

bool FHuntedPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
