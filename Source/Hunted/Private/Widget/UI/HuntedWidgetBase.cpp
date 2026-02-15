// KasaiRaito All Rights Reserved


#include "Widget/UI/HuntedWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"

void UHuntedWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UPlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent())
		{
			BP_OnOwningHeroUIComponentInitialized(PlayerUIComponent);
		}
	}
}
