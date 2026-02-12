// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInventoryWidget.generated.h"

class UCanvasPanel;
class UBorder;
class UBackgroundBlur;


UCLASS()
class HUNTED_API UPlayerInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UBorder* BackgroundBorder;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UBackgroundBlur* BackgroundBlur;
	
protected:
	virtual void NativeConstruct() override;
};
