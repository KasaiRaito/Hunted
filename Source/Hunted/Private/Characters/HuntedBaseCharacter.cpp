// KasaiRaito All Rights Reserved


#include "Characters/HuntedBaseCharacter.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "AbilitySystem/HuntedAttributeSet.h"

// Sets default values
AHuntedBaseCharacter::AHuntedBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	HuntedAbilitySystemComponent = CreateDefaultSubobject<UHuntedAbilitySystemComponent>(FName("HuntedAbilitySystemComponent"));
	HuntedAttributeSet = CreateDefaultSubobject<UHuntedAttributeSet>(FName("HuntedAttributeSet"));
}

UAbilitySystemComponent* AHuntedBaseCharacter::GetAbilitySystemComponent() const
{
	return GetHuntedAbilitySystemComponent();
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