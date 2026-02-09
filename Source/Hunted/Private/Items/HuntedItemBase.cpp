// KasaiRaito All Rights Reserved


#include "Items/HuntedItemBase.h"

#include "Components/SphereComponent.h"

// Sets default values
AHuntedItemBase::AHuntedItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemIteractionTriger"));
	WeaponCollisionSphere->SetupAttachment(GetRootComponent());
	WeaponCollisionSphere->SetSphereRadius(20.f);
	WeaponCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

// Called when the game starts or when spawned
void AHuntedItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHuntedItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

