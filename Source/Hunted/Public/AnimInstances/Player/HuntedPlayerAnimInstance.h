// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/HuntedCharacterAnimInstance.h"
#include "HuntedPlayerAnimInstance.generated.h"

class AHuntedPlayerCharacter;

/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedPlayerAnimInstance : public UHuntedCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	AHuntedPlayerCharacter* OwningPlayerCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f;
	
	float IdleElapsedTime;
};
