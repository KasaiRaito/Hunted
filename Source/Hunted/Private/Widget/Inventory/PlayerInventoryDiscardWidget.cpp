// KasaiRaito All Rights Reserved

#include "Widget/Inventory/PlayerInventoryDiscardWidget.h"

#include "Components/Button.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Components/TextBlock.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"

UPlayerInventoryDiscardWidget::UPlayerInventoryDiscardWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DiscardTitle = NSLOCTEXT("InventoryOverflow", "DiscardTitle", "Discard Items");
	ConfirmLabel = NSLOCTEXT("InventoryOverflow", "ConfirmLabel", "Confirm");
	CancelLabel = NSLOCTEXT("InventoryOverflow", "CancelLabel", "Cancel");
}

void UPlayerInventoryDiscardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsClosing = false;
	BindButtonCallbacks();
	ApplyText();
}

void UPlayerInventoryDiscardWidget::NativeDestruct()
{
	UnbindButtonCallbacks();

	if (!bIsClosing && IsValid(InventoryComponent) && InventoryComponent->IsFullInventoryResolutionActive())
	{
		InventoryComponent->CancelFullInventoryResolution();
	}

	Super::NativeDestruct();
}

void UPlayerInventoryDiscardWidget::ConfigureForOverflow(UPlayerInventoryComponent* InInventoryComponent,
	AHuntedInventoryItemBase* InPendingPickup)
{
	InventoryComponent = InInventoryComponent;
	PendingPickup = InPendingPickup;
	ApplyText();
	BP_OnOverflowConfigured(PendingPickup);
}

void UPlayerInventoryDiscardWidget::CancelResolution()
{
	if (bIsClosing)
	{
		return;
	}

	if (IsValid(InventoryComponent))
	{
		InventoryComponent->CancelFullInventoryResolution();
	}

	CloseResolution(false);
}

void UPlayerInventoryDiscardWidget::HandleConfirmClicked()
{
	if (!IsValid(InventoryComponent))
	{
		BP_OnConfirmationRejected();
		return;
	}

	if (!InventoryComponent->ConfirmFullInventoryResolution())
	{
		BP_OnConfirmationRejected();
		return;
	}

	const bool bPickupAccepted = !IsValid(PendingPickup) || PendingPickup->GetOwner() == InventoryComponent->GetOwner();
	CloseResolution(bPickupAccepted);
}

void UPlayerInventoryDiscardWidget::HandleCancelClicked()
{
	CancelResolution();
}

void UPlayerInventoryDiscardWidget::BindButtonCallbacks()
{
	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.RemoveDynamic(this, &UPlayerInventoryDiscardWidget::HandleConfirmClicked);
		ConfirmButton->OnClicked.AddDynamic(this, &UPlayerInventoryDiscardWidget::HandleConfirmClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.RemoveDynamic(this, &UPlayerInventoryDiscardWidget::HandleCancelClicked);
		CancelButton->OnClicked.AddDynamic(this, &UPlayerInventoryDiscardWidget::HandleCancelClicked);
	}
}

void UPlayerInventoryDiscardWidget::UnbindButtonCallbacks()
{
	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.RemoveDynamic(this, &UPlayerInventoryDiscardWidget::HandleConfirmClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.RemoveDynamic(this, &UPlayerInventoryDiscardWidget::HandleCancelClicked);
	}
}

void UPlayerInventoryDiscardWidget::CloseResolution(bool bPickupAccepted)
{
	if (bIsClosing)
	{
		return;
	}

	bIsClosing = true;
	BP_OnResolutionClosed(bPickupAccepted);
	OnResolutionClosed.Broadcast(bPickupAccepted);
	RemoveFromParent();
}

void UPlayerInventoryDiscardWidget::ApplyText()
{
	if (TitleText)
	{
		TitleText->SetText(DiscardTitle);
	}

	if (ConfirmButtonText)
	{
		ConfirmButtonText->SetText(ConfirmLabel);
	}

	if (CancelButtonText)
	{
		CancelButtonText->SetText(CancelLabel);
	}
}
