// KasaiRaito All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "HuntedGameplayTags.h"

#include "HuntedDebugHelper.h"

void UEnemyCombatComponent::OnWeaponHitTarget(AActor* HitActor)
{
	// Weapon overlaps can outlive the target during enemy death/despawn.
	if (!IsValid(GetOwningPawn()) || !IsValid(HitActor))
	{
		return;
	}

	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}
	
	OverlappedActors.AddUnique(HitActor);
	
	//TODO: Implement block check
	const bool bIsValidBlock = false;
	
	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnblockable = false;
	
	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		//TODO: check if block is valid
	}
	
	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;
	
	if (bIsValidBlock)
	{
		//TODO: handle successful block
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			HuntedGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}
