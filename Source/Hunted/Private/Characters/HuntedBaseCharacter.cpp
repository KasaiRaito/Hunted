// KasaiRaito All Rights Reserved


#include "Characters/HuntedBaseCharacter.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "AbilitySystem/HuntedAttributeSet.h"
#include "MotionWarpingComponent.h"
#include "ContextualAnimSceneActorComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "HuntedGameplayTags.h"

// Sets default values
AHuntedBaseCharacter::AHuntedBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	HuntedAbilitySystemComponent = CreateDefaultSubobject<UHuntedAbilitySystemComponent>(FName("HuntedAbilitySystemComponent"));
	HuntedAttributeSet = CreateDefaultSubobject<UHuntedAttributeSet>(FName("HuntedAttributeSet"));
	
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(FName("MotionWarpingComponent"));
}

UAbilitySystemComponent* AHuntedBaseCharacter::GetAbilitySystemComponent() const
{
	return GetHuntedAbilitySystemComponent();
}

UPawnCombatComponent* AHuntedBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AHuntedBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

void AHuntedBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HuntedAbilitySystemComponent)
	{
		HuntedAbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to Assign STARTUP DATA to %s"), *GetName());
	}
}