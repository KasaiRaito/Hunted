// KasaiRaito All Rights Reserved


#include "Items/Weapons/HuntedPlayerWeaponBase.h"

void AHuntedPlayerWeaponBase::AssignGratedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AHuntedPlayerWeaponBase::GetGrantedAbilitySpecHandle() const
{
	return GrantedAbilitySpecHandles;
}