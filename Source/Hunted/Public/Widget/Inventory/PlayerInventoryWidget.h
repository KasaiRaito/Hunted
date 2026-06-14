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
class UPlayerInventoryDiscardWidget;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Drop Popup")
	TSubclassOf<UPlayerInventoryDropPopupWidget> DropPopupWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Drop Popup")
	int32 DropPopupZOrder = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Inventory Overflow",
		meta = (DisplayName = "Discard Widget Class", ToolTip = "Widget Blueprint shown when a world pickup does not fit in the inventory."))
	TSubclassOf<UPlayerInventoryDiscardWidget> DiscardWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Inventory Overflow",
		meta = (DisplayName = "Discard Widget Z Order", ClampMin = "0"))
	int32 DiscardWidgetZOrder = 900;

protected:
	UPROPERTY(Transient)
	AHuntedPlayerCharacter* CharacterReference = nullptr;

	UPROPERTY(Transient)
	UPlayerInventoryDropPopupWidget* ActiveDropPopup = nullptr;

	UPROPERTY(Transient)
	UPlayerInventoryDiscardWidget* ActiveDiscardWidget = nullptr;

	bool bInventoryWasVisibleBeforeOverflow = false;
	bool bPreviousMouseCursorVisible = false;
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void HandleInventoryVisibilityChanged(ESlateVisibility InVisibility);

	void ShowDropPopupForItem(AHuntedInventoryItemBase* Item);
	void ClearActiveDropPopup();
	void ShowDiscardWidgetForPickup(AHuntedInventoryItemBase* PendingPickup);
	void RestoreInputAfterOverflow();

	UFUNCTION()
	void HandleDropRequested(AHuntedInventoryItemBase* Item);

	UFUNCTION()
	void HandleDropConfirmed(AHuntedInventoryItemBase* Item);

	UFUNCTION()
	void HandleDropPopupClosed();

	UFUNCTION()
	void HandleInventoryOverflowRequested(AHuntedInventoryItemBase* PendingPickup);

	UFUNCTION()
	void HandleOverflowResolutionClosed(bool bPickupAccepted);
};
