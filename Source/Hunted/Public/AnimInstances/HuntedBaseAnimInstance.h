// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimNodes/AnimNode_BlendSpacePlayer.h"
#include "HuntedBaseAnimInstance.generated.h"

class UBlendSpace;
class FObjectPropertyBase;

struct FHuntedBlendSpacePlayerCache
{
	FAnimNode_BlendSpacePlayerBase* Node = nullptr;
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

	// Linked weapon layers can replace a missing/invalid standing locomotion asset.
	virtual UBlendSpace* ResolveStandingLocomotionBlendSpace(
		UBlendSpace* ConfiguredBlendSpace) const;

private:
	void CacheLocomotionBlendSpacePlayers();
	void UpdateCrouchedLocomotionBlendSpaces();

	TArray<FHuntedBlendSpacePlayerCache> LocomotionBlendSpacePlayers;
	FObjectPropertyBase* DefaultLocomotionBlendSpaceProperty = nullptr;
	TWeakObjectPtr<UBlendSpace> DefaultLocomotionBlendSpace;
	bool bLocomotionBlendSpacesCached = false;
	bool bWasUsingCrouchedLocomotion = false;
};
