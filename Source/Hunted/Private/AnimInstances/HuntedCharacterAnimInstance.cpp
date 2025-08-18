// KasaiRaito All Rights Reserved


#include "AnimInstances/HuntedCharacterAnimInstance.h"
#include "Characters/HuntedBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	bHasAccelerations = (OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f);
}
