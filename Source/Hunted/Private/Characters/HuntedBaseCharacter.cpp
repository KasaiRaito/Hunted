// KasaiRaito All Rights Reserved


#include "Characters/HuntedBaseCharacter.h"

// Sets default values
AHuntedBaseCharacter::AHuntedBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;
}

