// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/HuntedBaseAnimInstance.h"
#include "HuntedCharacterAnimInstance.generated.h"

class AHuntedBaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedCharacterAnimInstance : public UHuntedBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	AHuntedBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAccelerations;
};
