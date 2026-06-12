// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimNodes/AnimNode_BlendSpacePlayer.h"
#include "HuntedBaseAnimInstance.generated.h"

class UBlendSpace;

struct FHuntedBlendSpacePlayerCache
{
	FAnimNode_BlendSpacePlayer* Node = nullptr;
	TWeakObjectPtr<UBlendSpace> DefaultBlendSpace;
};

/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UHuntedBaseAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	// All player locomotion layers reuse this asset while the owning character is crouched.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Crouch")
	UBlendSpace* CrouchedLocomotionBlendSpace;

private:
	void CacheLocomotionBlendSpacePlayers();
	void UpdateCrouchedLocomotionBlendSpaces();

	TArray<FHuntedBlendSpacePlayerCache> LocomotionBlendSpacePlayers;
	bool bWasUsingCrouchedLocomotion = false;
};
