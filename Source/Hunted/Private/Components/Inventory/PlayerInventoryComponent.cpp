// KasaiRaito All Rights Reserved


#include "Components/Inventory/PlayerInventoryComponent.h"
#include "HuntedDebugHelper.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Debug::Print(TEXT("Player Inventory Component BeginPlay"), FColor::Purple);
		
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

