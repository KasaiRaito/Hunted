// KasaiRaito All Rights Reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "HuntedTypes/HuntedStructTypes.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInventoryGrid.generated.h"

class UCanvasPanel;
class UBorder;

/**
 * 
 */
UCLASS()
class HUNTED_API UPlayerInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UBorder* GridBorder;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UCanvasPanel* GridCanvasPanel;
};
