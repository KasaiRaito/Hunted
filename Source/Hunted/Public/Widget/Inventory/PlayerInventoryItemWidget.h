// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widget/UI/HuntedWidgetBase.h"
#include "PlayerInventoryItemWidget.generated.h"

class UCanvasPanel;
class USizeBox;
class UBorder;
class UImage;
/**
 * 
 */
UCLASS()
class HUNTED_API UPlayerInventoryItemWidget : public UHuntedWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	USizeBox* BackgroundSizeBox;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UBorder* BackgroundBorder;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UImage* ItemImage;	
};
