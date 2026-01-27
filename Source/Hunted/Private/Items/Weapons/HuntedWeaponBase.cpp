// KasaiRaito All Rights Reserved


#include "Items/Weapons/HuntedWeaponBase.h"
#include "Components/BoxComponent.h"

#include "HuntedDebugHelper.h"

AHuntedWeaponBase::AHuntedWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisionBoxEndOverlap);
}

void AHuntedWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	
	checkf(WeaponOwningPawn, TEXT("Forgot instigator as Owning Pawn for the Weapon: %s "), *GetName());
	
	if (APawn* HitPawn= Cast<APawn>(OtherActor))
	{
		if (WeaponOwningPawn != HitPawn)
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
			//Debug::Print(GetName() + TEXT("beguin ovelap with ") + HitPawn->GetName(), FColor::Green);
		}
	}
}

void AHuntedWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	
	checkf(WeaponOwningPawn, TEXT("Forgot instigator as Owning Pawn for the Weapon: %s "), *GetName());
	
	if (APawn* HitPawn= Cast<APawn>(OtherActor))
	{
		if (WeaponOwningPawn != HitPawn)
		{
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
			//Debug::Print(GetName() + TEXT("end overlap with ") + HitPawn->GetName(), FColor::Red);
		}
	}	
}

void AHuntedWeaponBase::HandleRayCastHit(AActor* OtherActor)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	
	checkf(WeaponOwningPawn, TEXT("Forgot instigator as Owning Pawn for the Weapon: %s "), *GetName());
	
	if (APawn* HitPawn= Cast<APawn>(OtherActor))
	{
		if (WeaponOwningPawn != HitPawn)
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
			//Debug::Print(GetName() + TEXT("beguin ovelap with ") + HitPawn->GetName(), FColor::Green);
		}
	}
}
