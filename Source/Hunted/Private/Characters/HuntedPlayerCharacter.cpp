// KasaiRaito All Rights Reserved


#include "Characters/HuntedPlayerCharacter.h"

#include "WarriorDebugHelper.h"

void AHuntedPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print(TEXT("HuntedPlayerCharacter::BeginPlay"));
}
