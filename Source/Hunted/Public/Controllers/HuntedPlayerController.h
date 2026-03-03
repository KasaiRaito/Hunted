// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "HuntedPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HUNTED_API AHuntedPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AHuntedPlayerController();
	
	//~ Begin IGenericTeamInterface.
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamInterface.

private:
	FGenericTeamId PlayerTeamId;
	
};
