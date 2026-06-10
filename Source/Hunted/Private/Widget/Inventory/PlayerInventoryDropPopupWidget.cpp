// KasaiRaito All Rights Reserved


#include "Widget/Inventory/PlayerInventoryDropPopupWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"

UPlayerInventoryDropPopupWidget::UPlayerInventoryDropPopupWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DropConfirmationTitle = NSLOCTEXT("InventoryDropPopup", "DropConfirmationTitle", "Drop item?");
	DropConfirmationMessageFormat = NSLOCTEXT("InventoryDropPopup", "DropConfirmationMessageFormat", "{ItemName} will be destroyed and lost permanently.");
	RequiredItemTitle = NSLOCTEXT("InventoryDropPopup", "RequiredItemTitle", "Required item");
	RequiredItemMessageFormat = NSLOCTEXT("InventoryDropPopup", "RequiredItemMessageFormat", "{ItemName} is required and can't be dropped.");
	DropConfirmButtonLabel = NSLOCTEXT("InventoryDropPopup", "DropConfirmButtonLabel", "Drop");
	DropCancelButtonLabel = NSLOCTEXT("InventoryDropPopup", "DropCancelButtonLabel", "Cancel");
	RequiredItemConfirmButtonLabel = NSLOCTEXT("InventoryDropPopup", "RequiredItemConfirmButtonLabel", "OK");
}

void UPlayerInventoryDropPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsClosing = false;
	BindButtonCallbacks();
}

void UPlayerInventoryDropPopupWidget::NativeDestruct()
{
	UnbindButtonCallbacks();
	Super::NativeDestruct();
}

void UPlayerInventoryDropPopupWidget::ConfigureForItem(AHuntedInventoryItemBase* InItem, bool bInCanDrop)
{
	Item = InItem;
	bCanDrop = bInCanDrop;

	const FText Title = bCanDrop ? DropConfirmationTitle : RequiredItemTitle;
	const FText MessageFormat = bCanDrop ? DropConfirmationMessageFormat : RequiredItemMessageFormat;

	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("ItemName"), BuildItemNameText());

	const FText Message = FText::Format(MessageFormat, Arguments);
	ApplyConfiguredText(Title, Message);

	BP_OnPopupConfigured(Item, bCanDrop, Title, Message);
}

void UPlayerInventoryDropPopupWidget::ClosePopup()
{
	if (bIsClosing)
	{
		return;
	}

	bIsClosing = true;
	BP_OnPopupClosed();
	OnPopupClosed.Broadcast();
	RemoveFromParent();
}

void UPlayerInventoryDropPopupWidget::HandleConfirmClicked()
{
	if (bCanDrop && IsValid(Item))
	{
		OnDropConfirmed.Broadcast(Item);
	}

	ClosePopup();
}

void UPlayerInventoryDropPopupWidget::HandleCancelClicked()
{
	OnDropCanceled.Broadcast();
	ClosePopup();
}

void UPlayerInventoryDropPopupWidget::BindButtonCallbacks()
{
	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.RemoveDynamic(this, &UPlayerInventoryDropPopupWidget::HandleConfirmClicked);
		ConfirmButton->OnClicked.AddDynamic(this, &UPlayerInventoryDropPopupWidget::HandleConfirmClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.RemoveDynamic(this, &UPlayerInventoryDropPopupWidget::HandleCancelClicked);
		CancelButton->OnClicked.AddDynamic(this, &UPlayerInventoryDropPopupWidget::HandleCancelClicked);
	}
}

void UPlayerInventoryDropPopupWidget::UnbindButtonCallbacks()
{
	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.RemoveDynamic(this, &UPlayerInventoryDropPopupWidget::HandleConfirmClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.RemoveDynamic(this, &UPlayerInventoryDropPopupWidget::HandleCancelClicked);
	}
}

void UPlayerInventoryDropPopupWidget::ApplyConfiguredText(const FText& Title, const FText& Message)
{
	if (TitleText)
	{
		TitleText->SetText(Title);
	}

	if (MessageText)
	{
		MessageText->SetText(Message);
	}

	if (ConfirmButtonText)
	{
		ConfirmButtonText->SetText(bCanDrop ? DropConfirmButtonLabel : RequiredItemConfirmButtonLabel);
	}

	if (CancelButtonText)
	{
		CancelButtonText->SetText(DropCancelButtonLabel);
	}

	if (CancelButton)
	{
		CancelButton->SetVisibility(bCanDrop ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

FText UPlayerInventoryDropPopupWidget::BuildItemNameText() const
{
	if (!IsValid(Item))
	{
		return NSLOCTEXT("InventoryDropPopup", "FallbackItemName", "This item");
	}

	FString ItemName = Item->GetItemData().ItemName;
	ItemName.TrimStartAndEndInline();

	if (ItemName.IsEmpty())
	{
		return NSLOCTEXT("InventoryDropPopup", "FallbackItemName", "This item");
	}

	return FText::FromString(ItemName);
}
