// KasaiRaito All Rights Reserved


#include "Items/Inventory/HuntedInventoryItemBase.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/GameSession.h"
#include "Widget/Items/ItemInteractuableWidget.h"

AHuntedInventoryItemBase::AHuntedInventoryItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SpherCollider"));
	
	MeshComponent->SetupAttachment(RootComponent);
	CollisionSphere->SetupAttachment(MeshComponent);
	
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	InteractWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("InteractWidgetComponent");
	InteractWidgetComponent->SetupAttachment(MeshComponent);
}

void AHuntedInventoryItemBase::SetItemIsInVew(bool val)
{
	ItemIsInVew = val; 
	
	if (val)
	{
		ItemData.SetItemAmount();
	}
}

void AHuntedInventoryItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (UHuntedWidgetBase* ObjectWidget =  Cast<UItemInteractuableWidget>(InteractWidgetComponent->GetUserWidgetObject()))
	{
		ObjectWidget->InitInteractCreateWidget(this);
	}
}

void AHuntedInventoryItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}