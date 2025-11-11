// KasaiRaito All Rights Reserved


#include "AnimInstances/Player/HuntedPlayerAnimInstance.h"

#include "Characters/HuntedPlayerCharacter.h"

void UHuntedPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningPlayerCharacter = Cast<AHuntedPlayerCharacter>(OwningCharacter);
	}
}

void UHuntedPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElapsedTime >= EnterRelaxStateThreshold) ? true : false;
	}
}