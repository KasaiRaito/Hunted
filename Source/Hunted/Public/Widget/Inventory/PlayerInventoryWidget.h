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
class UPlayerInventoryComponent;
class UPlayerInventoryDropPopupWidget;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Drop Popup")
	TSubclassOf<UPlayerInventoryDropPopupWidget> DropPopupWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Drop Popup")
	int32 DropPopupZOrder = 1000;

	UPROPERTY(Transient)
	AHuntedPlayerCharacter* CharacterReference = nullptr;

	UPROPERTY(Transient)
	UPlayerInventoryDropPopupWidget* ActiveDropPopup = nullptr;
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void HandleInventoryVisibilityChanged(ESlateVisibility InVisibility);

	void ShowDropPopupForItem(AHuntedInventoryItemBase* Item);
	void ClearActiveDropPopup();

	UFUNCTION()
	void HandleDropRequested(AHuntedInventoryItemBase* Item);

	UFUNCTION()
	void HandleDropConfirmed(AHuntedInventoryItemBase* Item);

	UFUNCTION()
	void HandleDropPopupClosed();
};
