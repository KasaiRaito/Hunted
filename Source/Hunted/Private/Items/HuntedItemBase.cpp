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

	WeaponCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionSphere->SetupAttachment(GetRootComponent());
	WeaponCollisionSphere->SetSphereRadius(20.f);
	WeaponCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionSphere->OnComponentEndOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisionBoxEndOverlap);


}

// Called when the game starts or when spawned
void AHuntedItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHuntedItemBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AHuntedItemBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called every frame
void AHuntedItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

