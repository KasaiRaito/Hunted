// KasaiRaito All Rights Reserved


#include "AnimInstances/HuntedCharacterAnimInstance.h"
#include "Characters/HuntedBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "HuntedDebugHelper.h"
#include "Characters/HuntedPlayerCharacter.h"

void UHuntedCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AHuntedBaseCharacter>(TryGetPawnOwner());

	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UHuntedCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	FVector GlobalVelocity = OwningCharacter->GetVelocity();

	GroundSpeed = OwningCharacter->GetActorTransform().InverseTransformVectorNoScale(GlobalVelocity);

	bHasAcceleration = (OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f);
	
	//CurrentPitch = OwningCharacter->GetControlRotation().Pitch;
	
	if (AHuntedPlayerCharacter* Player = Cast<AHuntedPlayerCharacter>(OwningCharacter))
	{
		Aiming = Player->GetAimingState();
	}
}

