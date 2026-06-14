// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widget/UI/HuntedWidgetBase.h"
#include "PlayerInventoryDiscardWidget.generated.h"

class AHuntedInventoryItemBase;
class UButton;
class UPlayerInventoryComponent;
class UPlayerInventoryDiscardGridWidget;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryOverflowWidgetClosedDelegate, bool, bPickupAccepted);

UCLASS(BlueprintType, Blueprintable)
class HUNTED_API UPlayerInventoryDiscardWidget : public UHuntedWidgetBase
{
	GENERATED_BODY()

public:
	UPlayerInventoryDiscardWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, Category = "Inventory|Overflow")
	FInventoryOverflowWidgetClosedDelegate OnResolutionClosed;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Overflow")
	void ConfigureForOverflow(UPlayerInventoryComponent* InInventoryComponent,
		AHuntedInventoryItemBase* InPendingPickup);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Overflow")
	void CancelResolution();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Overflow")
	UPlayerInventoryDiscardGridWidget* DiscardGrid = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Overflow")
	UTextBlock* TitleText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Overflow")
	UTextBlock* ConfirmButtonText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Overflow")
	UTextBlock* CancelButtonText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Overflow")
	UButton* ConfirmButton = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Overflow")
	UButton* CancelButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Overflow|Text")
	FText DiscardTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Overflow|Text")
	FText ConfirmLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Overflow|Text")
	FText CancelLabel;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Inventory|Overflow")
	UPlayerInventoryComponent* InventoryComponent = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Inventory|Overflow")
	AHuntedInventoryItemBase* PendingPickup = nullptr;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|Overflow")
	void BP_OnOverflowConfigured(AHuntedInventoryItemBase* ConfiguredPendingPickup);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|Overflow")
	void BP_OnConfirmationRejected();

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|Overflow")
	void BP_OnResolutionClosed(bool bPickupAccepted);

private:
	bool bIsClosing = false;

	UFUNCTION()
	void HandleConfirmClicked();

	UFUNCTION()
	void HandleCancelClicked();

	void BindButtonCallbacks();
	void UnbindButtonCallbacks();
	void CloseResolution(bool bPickupAccepted);
	void ApplyText();
};
