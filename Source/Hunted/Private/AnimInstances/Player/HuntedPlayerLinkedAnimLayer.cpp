// KasaiRaito All Rights Reserved


#include "AnimInstances/Player/HuntedPlayerLinkedAnimLayer.h"

#include "Animation/BlendSpace.h"
#include "AnimInstances/Player/HuntedPlayerAnimInstance.h"
#include "Characters/HuntedPlayerCharacter.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "HuntedGameplayTags.h"
#include "UObject/ConstructorHelpers.h"

UHuntedPlayerLinkedAnimLayer::UHuntedPlayerLinkedAnimLayer()
{
	static ConstructorHelpers::FObjectFinder<UBlendSpace> UnarmedLocomotionAsset(
		TEXT("/Game/_Hunted/PlayerCharacter/AnimBPs/BlendSpace/"
			"BS_UnarmedLocomotion.BS_UnarmedLocomotion"));

	UnarmedLocomotionBlendSpace = UnarmedLocomotionAsset.Object;
}

UHuntedPlayerAnimInstance* UHuntedPlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UHuntedPlayerAnimInstance>( GetOwningComponent()->GetAnimInstance());
}

UBlendSpace* UHuntedPlayerLinkedAnimLayer::ResolveStandingLocomotionBlendSpace(
	UBlendSpace* ConfiguredBlendSpace) const
{
	const AHuntedPlayerCharacter* PlayerCharacter =
		Cast<AHuntedPlayerCharacter>(TryGetPawnOwner());
	const UPlayerCombatComponent* CombatComponent =
		PlayerCharacter ? PlayerCharacter->GetPlayerCombatComponent() : nullptr;

	const bool bKnifeIsEquipped =
		CombatComponent &&
		CombatComponent->CurrentEquippedWeaponTag ==
			HuntedGameplayTags::Player_Weapon_BasicKnife;
	const bool bUsesEmptyKnifeLocomotion =
		ConfiguredBlendSpace &&
		ConfiguredBlendSpace->GetFName() == TEXT("BS_KnifeLocomotion");

	// Keep the knife's linked layer for attack montages, but fall back to the complete
	// unarmed locomotion set. The base class still replaces it with crouch locomotion.
	if ((bKnifeIsEquipped || bUsesEmptyKnifeLocomotion) &&
		IsValid(UnarmedLocomotionBlendSpace))
	{
		return UnarmedLocomotionBlendSpace;
	}

	return Super::ResolveStandingLocomotionBlendSpace(ConfiguredBlendSpace);
}
