// KasaiRaito All Rights Reserved

#include "HuntedTypes/HuntedStructTypes.h"
#include "AbilitySystem/Abilities/HuntedGameplayAbility.h"
#include "Kismet/KismetMathLibrary.h"

bool FHuntedPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}

void FHuntedPlayerItemData::SetItemAmount()
{
	ItemAmount = FMath::RandRange(ItemMinAmount.GetValue(), ItemMaxAmount.GetValue());
}
