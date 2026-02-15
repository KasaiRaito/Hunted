// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HuntedWidgetBase.generated.h"

class UPlayerUIComponent;
/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnOwningHeroUIComponentInitialized"))
	void BP_OnOwningHeroUIComponentInitialized(UPlayerUIComponent* OwningPlayerUIComponent);
};
