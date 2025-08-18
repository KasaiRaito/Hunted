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

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	bHasAccelerations = (OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f);
}
