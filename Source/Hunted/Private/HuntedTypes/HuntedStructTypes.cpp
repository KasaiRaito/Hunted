// KasaiRaito All Rights Reserved

#include "HuntedTypes/HuntedStructTypes.h"
#include "AbilitySystem/Abilities/HuntedGameplayAbility.h"

bool FHuntedPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}

bool FHuntedPlayerItemData::IsValid() const
{
	if (!ItemName.IsEmpty() && !ItemDescription.IsEmpty() &&
		&ItemMesh != nullptr && ItemImage != nullptr && ItemAmount > 0 &&
		ItemTag != FGameplayTag::EmptyTag)
	{
		return true;
	}
	
	return false;
}