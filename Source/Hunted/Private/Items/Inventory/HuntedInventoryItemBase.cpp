// KasaiRaito All Rights Reserved


#include "Items/Inventory/HuntedInventoryItemBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/GameSession.h"

AHuntedInventoryItemBase::AHuntedInventoryItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SpherCollider"));
	
	MeshComponent->SetupAttachment(RootComponent);
	CollisionSphere->SetupAttachment(MeshComponent);
	
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHuntedInventoryItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHuntedInventoryItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}