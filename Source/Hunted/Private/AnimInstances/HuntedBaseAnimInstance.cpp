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

	// NativeInitializeAnimation runs before Unreal initializes the root AnimGraph nodes.
	// Defer the cache until the first update so their original BlendSpaces are available.
	LocomotionBlendSpacePlayers.Reset();
	DefaultLocomotionBlendSpaceProperty = nullptr;
	DefaultLocomotionBlendSpace.Reset();
	bLocomotionBlendSpacesCached = false;
	bWasUsingCrouchedLocomotion = false;
}

void UHuntedBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	UpdateCrouchedLocomotionBlendSpaces();
}

UBlendSpace* UHuntedBaseAnimInstance::ResolveStandingLocomotionBlendSpace(
	UBlendSpace* ConfiguredBlendSpace) const
{
	return ConfiguredBlendSpace;
}

void UHuntedBaseAnimInstance::CacheLocomotionBlendSpacePlayers()
{
	LocomotionBlendSpacePlayers.Reset();
	DefaultLocomotionBlendSpaceProperty = nullptr;
	DefaultLocomotionBlendSpace.Reset();

	const IAnimClassInterface* AnimClassInterface = IAnimClassInterface::GetFromClass(GetClass());
	if (!AnimClassInterface)
	{
		return;
	}

	for (FStructProperty* NodeProperty : AnimClassInterface->GetAnimNodeProperties())
	{
		// Rotation-offset BlendSpaces are aim offsets, so only cache exact locomotion player nodes.
		if (!NodeProperty)
		{
			continue;
		}

		FAnimNode_BlendSpacePlayerBase* BlendSpacePlayer = nullptr;
		if (NodeProperty->Struct == FAnimNode_BlendSpacePlayer::StaticStruct())
		{
			BlendSpacePlayer = NodeProperty->ContainerPtrToValuePtr<FAnimNode_BlendSpacePlayer>(this);
		}
		else if (NodeProperty->Struct == FAnimNode_BlendSpacePlayer_Standalone::StaticStruct())
		{
			BlendSpacePlayer =
				NodeProperty->ContainerPtrToValuePtr<FAnimNode_BlendSpacePlayer_Standalone>(this);
		}

		if (!BlendSpacePlayer || !BlendSpacePlayer->GetBlendSpace())
		{
			continue;
		}

		UBlendSpace* StandingBlendSpace =
			ResolveStandingLocomotionBlendSpace(BlendSpacePlayer->GetBlendSpace());

		FHuntedBlendSpacePlayerCache& CacheEntry = LocomotionBlendSpacePlayers.AddDefaulted_GetRef();
		CacheEntry.Node = BlendSpacePlayer;
		CacheEntry.DefaultBlendSpace = StandingBlendSpace;

		if (IsValid(StandingBlendSpace) &&
			StandingBlendSpace != BlendSpacePlayer->GetBlendSpace())
		{
			BlendSpacePlayer->SetBlendSpace(StandingBlendSpace);
		}
	}

	// Weapon linked layers feed their BlendSpace node from this Blueprint variable every frame.
	// Updating the variable prevents that exposed input from overwriting the crouch asset.
	DefaultLocomotionBlendSpaceProperty =
		FindFProperty<FObjectPropertyBase>(GetClass(), TEXT("DefaultLocomotionBlendSpace"));
	if (DefaultLocomotionBlendSpaceProperty &&
		DefaultLocomotionBlendSpaceProperty->PropertyClass->IsChildOf(UBlendSpace::StaticClass()))
	{
		UBlendSpace* ConfiguredBlendSpace = Cast<UBlendSpace>(
			DefaultLocomotionBlendSpaceProperty->GetObjectPropertyValue_InContainer(this));
		DefaultLocomotionBlendSpace =
			ResolveStandingLocomotionBlendSpace(ConfiguredBlendSpace);

		if (DefaultLocomotionBlendSpace.IsValid() &&
			DefaultLocomotionBlendSpace.Get() != ConfiguredBlendSpace)
		{
			DefaultLocomotionBlendSpaceProperty->SetObjectPropertyValue_InContainer(
				this, DefaultLocomotionBlendSpace.Get());
		}
	}
	else
	{
		DefaultLocomotionBlendSpaceProperty = nullptr;
	}
}

void UHuntedBaseAnimInstance::UpdateCrouchedLocomotionBlendSpaces()
{
	if (!bLocomotionBlendSpacesCached)
	{
		CacheLocomotionBlendSpacePlayers();
		bLocomotionBlendSpacesCached = true;
	}

	const ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner());
	const bool bUseCrouchedLocomotion =
		Character && Character->bIsCrouched && IsValid(CrouchedLocomotionBlendSpace);

	// ABP_Player layers UpperBodySlot from spine_01, so weapon montages can run while this
	// crouch BlendSpace continues to own the pelvis and legs.
	if (bUseCrouchedLocomotion == bWasUsingCrouchedLocomotion)
	{
		return;
	}

	if (DefaultLocomotionBlendSpaceProperty)
	{
		UBlendSpace* TargetBlendSpace = bUseCrouchedLocomotion
			? CrouchedLocomotionBlendSpace
			: DefaultLocomotionBlendSpace.Get();

		if (IsValid(TargetBlendSpace))
		{
			DefaultLocomotionBlendSpaceProperty->SetObjectPropertyValue_InContainer(
				this, TargetBlendSpace);
		}
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
