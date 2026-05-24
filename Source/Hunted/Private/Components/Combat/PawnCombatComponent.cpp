// KasaiRaito All Rights Reserved


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/HuntedWeaponBase.h"
#include "Components/BoxComponent.h"

#include "HuntedDebugHelper.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister,
                                                 AHuntedWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	if (!InWeaponTagToRegister.IsValid() || !IsValid(InWeaponToRegister))
	{
		UE_LOG(LogTemp, Warning, TEXT("Skipped weapon registration because the tag or weapon pointer was invalid."));
		return;
	}

	if (AHuntedWeaponBase** ExistingWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToRegister))
	{
		if (*ExistingWeapon == InWeaponToRegister)
		{
			if (bRegisterAsEquippedWeapon)
			{
				CurrentEquippedWeaponTag = InWeaponTagToRegister;
			}
			return;
		}

		// Prevents repeated spawn abilities from crashing PIE when they reuse the same gameplay tag.
		if (IsValid(*ExistingWeapon))
		{
			(*ExistingWeapon)->OnWeaponHitTarget.Unbind();
			(*ExistingWeapon)->OnWeaponPulledFromTarget.Unbind();
		}
		*ExistingWeapon = InWeaponToRegister;
	}
	else
	{
		CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);
	}

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
			return IsValid(*FoundWeapon) ? *FoundWeapon : nullptr;
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
		
		// Animation notifies can fire after an ability ends or a weapon is replaced; skip instead of crashing.
		if (!IsValid(WeaponToToggle) || !WeaponToToggle->GetWeaponCollisionBox())
		{
			return;
		}
		
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
	//Debug::Print(TEXT(""), FColor::Red);
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	//Debug::Print(TEXT(""), FColor::Red);
}
