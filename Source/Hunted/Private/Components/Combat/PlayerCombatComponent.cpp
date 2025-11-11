// KasaiRaito All Rights Reserved


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items/Weapons/HuntedPlayerWeaponBase.h"

AHuntedPlayerWeaponBase* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<AHuntedPlayerWeaponBase>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}
