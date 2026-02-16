// KasaiRaito All Rights Reserved


#include "Widget/UI/HuntedWidgetBase.h"

#include "Components/UI/EnemyUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

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
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();
		
		checkf(EnemyUIComponent, TEXT("Failed to extract an EnemyUIComponent from %s"), *OwningEnemyActor->GetActorNameOrLabel());
		
		BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
	}
#endif
}
