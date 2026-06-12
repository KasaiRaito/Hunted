// KasaiRaito All Rights Reserved


#include "AnimInstances/HuntedBaseAnimInstance.h"

#include "Animation/AnimClassInterface.h"
#include "Animation/BlendSpace.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UnrealType.h"

UHuntedBaseAnimInstance::UHuntedBaseAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UBlendSpace> CrouchedLocomotionAsset(
		TEXT("/Game/_Hunted/PlayerCharacter/AnimBPs/BlendSpace/"
			"BS_Crouched_Locomotion.BS_Crouched_Locomotion"));

	CrouchedLocomotionBlendSpace = CrouchedLocomotionAsset.Object;
}

void UHuntedBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CacheLocomotionBlendSpacePlayers();
}

void UHuntedBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	UpdateCrouchedLocomotionBlendSpaces();
}

void UHuntedBaseAnimInstance::CacheLocomotionBlendSpacePlayers()
{
	LocomotionBlendSpacePlayers.Reset();

	const IAnimClassInterface* AnimClassInterface = IAnimClassInterface::GetFromClass(GetClass());
	if (!AnimClassInterface)
	{
		return;
	}

	for (FStructProperty* NodeProperty : AnimClassInterface->GetAnimNodeProperties())
	{
		// Rotation-offset BlendSpaces are aim offsets, so only cache exact locomotion player nodes.
		if (!NodeProperty || NodeProperty->Struct != FAnimNode_BlendSpacePlayer::StaticStruct())
		{
			continue;
		}

		FAnimNode_BlendSpacePlayer* BlendSpacePlayer =
			NodeProperty->ContainerPtrToValuePtr<FAnimNode_BlendSpacePlayer>(this);
		if (!BlendSpacePlayer || !BlendSpacePlayer->GetBlendSpace())
		{
			continue;
		}

		FHuntedBlendSpacePlayerCache& CacheEntry = LocomotionBlendSpacePlayers.AddDefaulted_GetRef();
		CacheEntry.Node = BlendSpacePlayer;
		CacheEntry.DefaultBlendSpace = BlendSpacePlayer->GetBlendSpace();
	}
}

void UHuntedBaseAnimInstance::UpdateCrouchedLocomotionBlendSpaces()
{
	const ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner());
	const bool bUseCrouchedLocomotion =
		Character && Character->bIsCrouched && IsValid(CrouchedLocomotionBlendSpace);

	if (bUseCrouchedLocomotion == bWasUsingCrouchedLocomotion)
	{
		return;
	}

	// Linked weapon layers pass through this same base class, so Gun and Knife restore their own
	// original locomotion BlendSpaces instead of being overwritten by one shared default.
	for (const FHuntedBlendSpacePlayerCache& CacheEntry : LocomotionBlendSpacePlayers)
	{
		if (!CacheEntry.Node)
		{
			continue;
		}

		UBlendSpace* TargetBlendSpace = bUseCrouchedLocomotion
			? CrouchedLocomotionBlendSpace
			: CacheEntry.DefaultBlendSpace.Get();

		if (IsValid(TargetBlendSpace))
		{
			CacheEntry.Node->SetBlendSpace(TargetBlendSpace);
		}
	}

	bWasUsingCrouchedLocomotion = bUseCrouchedLocomotion;
}
