// KasaiRaito All Rights Reserved


#include "Items/Inventory/HutedInventoryItemBase.h"

#include "Components/SphereComponent.h"

AHutedInventoryItemBase::AHutedInventoryItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SpherCollider"));
	
	MeshComponent->SetupAttachment(RootComponent);
	CollisionSphere->SetupAttachment(MeshComponent);
	
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHutedInventoryItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHutedInventoryItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

