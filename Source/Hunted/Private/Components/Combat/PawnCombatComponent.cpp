// KasaiRaito All Rights Reserved


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/HuntedWeaponBase.h"
#include "Components/BoxComponent.h"

#include "HuntedDebugHelper.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister,
                                                 AHuntedWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister),
		TEXT("A TAG named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString());

	check(InWeaponToRegister);

	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnWeaponHitTarget);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);
	
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}

	const FString WeaponString = FString::Printf(TEXT("A weapon named: %s has been registered using the tag %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	Debug::Print(WeaponString);
}

AHuntedWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		if (AHuntedWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

AHuntedWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AHuntedWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();
		
		check(WeaponToToggle);
		
		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			//Debug::Print(WeaponToToggle->GetName()+ TEXT(": collision enabled"), FColor::Green);
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OverlappedActors.Empty();
			//Debug::Print(WeaponToToggle->GetName()+ TEXT(": collision disabled"), FColor::Red);
		}
	}
}

void UPawnCombatComponent::OnWeaponHitTarget(AActor* HitActor)
{
	Debug::Print(TEXT(""), FColor::Red);
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	Debug::Print(TEXT(""), FColor::Red);
}
