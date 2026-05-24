// KasaiRaito All Rights Reserved


#include "Widget/UI/HuntedWidgetBase.h"

#include "Components/UI/EnemyUIComponent.h"
#include "Components/UI/ObjectUIComponent.h"
#include "Interfaces/PawnUIInterface.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"
#include "Interfaces/ItemUIInterface.h"


void UHuntedWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UPlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent())
		{
			BP_OnOwningPlayerUIComponentInitialized(PlayerUIComponent);
		}
	}
}

void UHuntedWidgetBase::InitEnemyCreateWidget(AActor* OwningEnemyActor)
{
#if WITH_EDITOR 
	if (!IsValid(OwningEnemyActor))
	{
		return;
	}

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();
		
		if (!IsValid(EnemyUIComponent))
		{
			// Editor preview widgets can be initialized before enemy components are ready.
			return;
		}
		
		BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
	}
#endif
}

void UHuntedWidgetBase::InitInteractCreateWidget(AActor* OwningObjectActor)
{
	if (!IsValid(OwningObjectActor))
	{
		return;
	}

	if (IItemUIInterface* ItemUIInterface = Cast<IItemUIInterface>(OwningObjectActor))
	{
		UObjectUIComponent* ObjectUIComponent = ItemUIInterface->GetObjectUIComponent();
		
		if (!IsValid(ObjectUIComponent))
		{
			// Interactable widgets may outlive the actor component during teardown.
			return;
		}
		
		BP_OnOwningObjectUIComponentInitialized(ObjectUIComponent);
	}
}
