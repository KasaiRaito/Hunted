// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/HuntedBaseAnimInstance.h"
#include "HuntedPlayerLinkedAnimLayer.generated.h"

class UHuntedPlayerAnimInstance;
/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedPlayerLinkedAnimLayer : public UHuntedBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (NotBlueprintThreadSafe))
	UHuntedPlayerAnimInstance* GetPlayerAnimInstance() const;
};
