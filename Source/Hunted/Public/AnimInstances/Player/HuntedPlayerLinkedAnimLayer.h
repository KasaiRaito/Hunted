// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/HuntedBaseAnimInstance.h"
#include "HuntedPlayerLinkedAnimLayer.generated.h"

class UHuntedPlayerAnimInstance;
class UBlendSpace;
/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedPlayerLinkedAnimLayer : public UHuntedBaseAnimInstance
{
	GENERATED_BODY()

public:
	UHuntedPlayerLinkedAnimLayer();

	UFUNCTION(BlueprintPure, meta = (NotBlueprintThreadSafe))
	UHuntedPlayerAnimInstance* GetPlayerAnimInstance() const;

protected:
	virtual UBlendSpace* ResolveStandingLocomotionBlendSpace(
		UBlendSpace* ConfiguredBlendSpace) const override;

private:
	UPROPERTY()
	UBlendSpace* UnarmedLocomotionBlendSpace;
};
