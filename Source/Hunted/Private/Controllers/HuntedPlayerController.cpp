// KasaiRaito All Rights Reserved


#include "Controllers/HuntedPlayerController.h"

AHuntedPlayerController::AHuntedPlayerController()
{
	PlayerTeamId = FGenericTeamId(0);
}

FGenericTeamId AHuntedPlayerController::GetGenericTeamId() const
{
	return PlayerTeamId;
}
