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

UPawnCombatComponent* AHuntedBaseCharacter::GetPawnCombatComponent() const
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

AActor* AHuntedBaseCharacter::HuntedRayCast(FVector startLocation, FVector rotation, float range)
{
	if (!GetWorld())
	{
		return nullptr;
	}
	
	FVector start = startLocation;
	FVector forward = rotation;
	
	start = FVector(start.X + (forward.X * 100),
		start.Y + (forward.Y * 100),
		start.Z + (forward.Z * 100));
	
	FVector end = start + (forward * range);
	
	FHitResult hit;
	
	
	bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
	
	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 1.f, 0.f, 10.f);
	
	if (actorHit && hit.GetActor())
	{
		GEngine->AddOnScreenDebugMessage(-1,2.0f, FColor::Green, hit.GetActor()->GetFName().ToString());
		return hit.GetActor();
	}
	
	return nullptr;
}
