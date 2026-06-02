// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widget/UI/HuntedWidgetBase.h"
#include "PlayerInventoryWidget.generated.h"

class UBorder;
class UCanvasPanel;
class UBackgroundBlur;
class AHuntedPlayerCharacter;
class AHuntedInventoryItemBase;

UCLASS()
class HUNTED_API UPlayerInventoryWidget : public UHuntedWidgetBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UBorder* BackgroundBorder;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UBackgroundBlur* BackgroundBlur;
	
	UPROPERTY(Transient)
	AHuntedInventoryItemBase* SpawnedItem = nullptr;
	
protected:
	UPROPERTY(Transient)
	AHuntedPlayerCharacter* CharacterReference = nullptr;
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void HandleInventoryVisibilityChanged(ESlateVisibility InVisibility);
	
	FHitResult GetLocationBelow(FVector Start) const;
};
