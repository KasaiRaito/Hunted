// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widget/UI/HuntedWidgetBase.h"
#include "PlayerInventoryDropPopupWidget.generated.h"

class AHuntedInventoryItemBase;
class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryDropPopupItemDelegate, AHuntedInventoryItemBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryDropPopupSimpleDelegate);

UCLASS(BlueprintType, Blueprintable)
class HUNTED_API UPlayerInventoryDropPopupWidget : public UHuntedWidgetBase
{
	GENERATED_BODY()

public:
	UPlayerInventoryDropPopupWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Drop Popup")
	FInventoryDropPopupItemDelegate OnDropConfirmed;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Drop Popup")
	FInventoryDropPopupSimpleDelegate OnPopupClosed;

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Drop Popup")
	FInventoryDropPopupSimpleDelegate OnDropCanceled;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Drop Popup")
	void ConfigureForItem(AHuntedInventoryItemBase* InItem, bool bInCanDrop);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Drop Popup")
	void ClosePopup();

	UFUNCTION(BlueprintPure, Category = "Inventory|Drop Popup")
	AHuntedInventoryItemBase* GetPopupItem() const { return Item; }

	UFUNCTION(BlueprintPure, Category = "Inventory|Drop Popup")
	bool IsDropConfirmationPopup() const { return bCanDrop; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Drop Popup")
	UTextBlock* TitleText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Drop Popup")
	UTextBlock* MessageText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Drop Popup")
	UTextBlock* ConfirmButtonText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Drop Popup")
	UTextBlock* CancelButtonText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Drop Popup")
	UButton* ConfirmButton = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Drop Popup")
	UButton* CancelButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Drop Popup|Text")
	FText DropConfirmationTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Drop Popup|Text")
	FText DropConfirmationMessageFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Drop Popup|Text")
	FText RequiredItemTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Drop Popup|Text")
	FText RequiredItemMessageFormat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Drop Popup|Text")
	FText DropConfirmButtonLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Drop Popup|Text")
	FText DropCancelButtonLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Drop Popup|Text")
	FText RequiredItemConfirmButtonLabel;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Inventory|Drop Popup")
	AHuntedInventoryItemBase* Item = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Inventory|Drop Popup")
	bool bCanDrop = false;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|Drop Popup")
	void BP_OnPopupConfigured(AHuntedInventoryItemBase* ConfiguredItem, bool bConfiguredCanDrop, const FText& Title, const FText& Message);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|Drop Popup")
	void BP_OnPopupClosed();

private:
	bool bIsClosing = false;

	UFUNCTION()
	void HandleConfirmClicked();

	UFUNCTION()
	void HandleCancelClicked();

	void BindButtonCallbacks();
	void UnbindButtonCallbacks();
	void ApplyConfiguredText(const FText& Title, const FText& Message);
	FText BuildItemNameText() const;
};
