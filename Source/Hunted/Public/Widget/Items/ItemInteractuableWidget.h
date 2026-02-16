// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widget/UI/HuntedWidgetBase.h"
#include "ItemInteractuableWidget.generated.h"

/**
 * 
 */
UCLASS()
class HUNTED_API UItemInteractuableWidget : public UHuntedWidgetBase
{
	GENERATED_BODY()
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
