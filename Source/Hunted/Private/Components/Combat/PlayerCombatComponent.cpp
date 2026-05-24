// KasaiRaito All Rights Reserved


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items/Weapons/HuntedPlayerWeaponBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "HuntedGameplayTags.h"

#include "HuntedDebugHelper.h"

AHuntedPlayerWeaponBase* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<AHuntedPlayerWeaponBase>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AHuntedPlayerWeaponBase* UPlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
	return Cast<AHuntedPlayerWeaponBase>(GetCharacterCurrentEquippedWeapon());
}

float UPlayerCombatComponent::GetPlayerCurrentEquippWeaponDamageAtLevel(float InLevel) const
{
	const AHuntedPlayerWeaponBase* CurrentWeapon = GetPlayerCurrentEquippedWeapon();
	return IsValid(CurrentWeapon) ? CurrentWeapon->PlayerWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel) : 0.0f;
}

void UPlayerCombatComponent::OnWeaponHitTarget(AActor* HitActor)
{
	// Overlap callbacks can arrive during teardown; ignore stale actors instead of dereferencing them.
	if (!IsValid(GetOwningPawn()) || !IsValid(HitActor))
	{
		return;
	}

	Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT(" hit ") + HitActor->GetActorNameOrLabel(), FColor::Green);

	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.Add(HitActor);

	FGameplayEventData HitData;
	HitData.Instigator = GetOwningPawn();
	HitData.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		HuntedGameplayTags::Shared_Event_MeleeHit,
		HitData
	);
	Super::OnWeaponHitTarget(HitActor);
}

void UPlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	Super::OnWeaponPulledFromTargetActor(InteractedActor);
}
