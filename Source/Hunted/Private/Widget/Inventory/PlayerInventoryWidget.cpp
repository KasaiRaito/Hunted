// KasaiRaito All Rights Reserved


#include "Widget/Inventory/PlayerInventoryWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Characters/HuntedPlayerCharacter.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"
#include "Widget/Inventory/PlayerInventoryGridWidget.h"
#include "Widget/Inventory/PlayerInventoryDiscardWidget.h"
#include "Widget/Inventory/PlayerInventoryDropPopupWidget.h"
#include "HuntedDebugHelper.h"

void UPlayerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CharacterReference = Cast<AHuntedPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (UPlayerInventoryComponent* InventoryComponent = IsValid(CharacterReference)
		? CharacterReference->GetPlayerInventoryComponent()
		: nullptr)
	{
		InventoryComponent->OnItemDropRequested.RemoveAll(this);
		InventoryComponent->OnItemDropRequested.AddDynamic(this, &UPlayerInventoryWidget::HandleDropRequested);
		InventoryComponent->OnInventoryOverflowRequested.RemoveAll(this);
		InventoryComponent->OnInventoryOverflowRequested.AddDynamic(
			this, &UPlayerInventoryWidget::HandleInventoryOverflowRequested);
	}

	OnNativeVisibilityChanged.RemoveAll(this);
	OnNativeVisibilityChanged.AddUObject(this, &UPlayerInventoryWidget::HandleInventoryVisibilityChanged);
	HandleInventoryVisibilityChanged(GetVisibility());
}

void UPlayerInventoryWidget::NativeDestruct()
{
	OnNativeVisibilityChanged.RemoveAll(this);

	if (UPlayerInventoryComponent* InventoryComponent = IsValid(CharacterReference)
		? CharacterReference->GetPlayerInventoryComponent()
		: nullptr)
	{
		InventoryComponent->OnItemDropRequested.RemoveAll(this);
		InventoryComponent->OnInventoryOverflowRequested.RemoveAll(this);

		if (InventoryComponent->IsFullInventoryResolutionActive())
		{
			InventoryComponent->CancelFullInventoryResolution();
		}
	}

	if (ActiveDiscardWidget)
	{
		ActiveDiscardWidget->OnResolutionClosed.RemoveAll(this);
		ActiveDiscardWidget->RemoveFromParent();
		ActiveDiscardWidget = nullptr;
	}

	ClearActiveDropPopup();
	Super::NativeDestruct();
}

FReply UPlayerInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

bool UPlayerInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (!InOperation || !IsValid(InOperation->Payload) || !IsValid(CharacterReference))
	{
		return false;
	}

	AHuntedInventoryItemBase* PayloadItem = Cast<AHuntedInventoryItemBase>(InOperation->Payload);
	if (!IsValid(PayloadItem))
	{
		return false;
	}

	UPlayerInventoryComponent* InventoryComponent = CharacterReference
		? CharacterReference->GetPlayerInventoryComponent()
		: nullptr;
	if (!IsValid(InventoryComponent))
	{
		return false;
	}

	if (InventoryComponent->IsFullInventoryResolutionActive())
	{
		return false;
	}

	SpawnedItem = nullptr;
	InOperation->Tag = TEXT("DropRequested");
	InventoryComponent->RequestDropItem(PayloadItem);
	
	return true;
}

void UPlayerInventoryWidget::HandleInventoryVisibilityChanged(ESlateVisibility InVisibility)
{
	UPlayerInventoryComponent* InventoryComponent = IsValid(CharacterReference)
		? CharacterReference->GetPlayerInventoryComponent()
		: nullptr;
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	if (InventoryComponent->IsFullInventoryResolutionActive()
		&& (InVisibility == ESlateVisibility::Collapsed || InVisibility == ESlateVisibility::Hidden))
	{
		SetVisibility(ESlateVisibility::Visible);
		return;
	}

	if (UPlayerInventoryGridWidget* InventoryGrid = InventoryComponent->GetPlayerInventoryGridWidget())
	{
		InventoryGrid->ResetItemInfoPanel();
	}
}

void UPlayerInventoryWidget::ShowDropPopupForItem(AHuntedInventoryItemBase* Item)
{
	if (!IsValid(Item))
	{
		return;
	}

	ClearActiveDropPopup();

	if (!DropPopupWidgetClass)
	{
		Debug::Print(TEXT("Player Inventory Widget: DropPopupWidgetClass is not assigned"), FColor::Red);
		return;
	}

	ActiveDropPopup = CreateWidget<UPlayerInventoryDropPopupWidget>(GetWorld(), DropPopupWidgetClass);
	if (!ActiveDropPopup)
	{
		return;
	}

	ActiveDropPopup->SetOwningPlayer(GetOwningPlayer());
	ActiveDropPopup->OnDropConfirmed.AddDynamic(this, &UPlayerInventoryWidget::HandleDropConfirmed);
	ActiveDropPopup->OnPopupClosed.AddDynamic(this, &UPlayerInventoryWidget::HandleDropPopupClosed);
	ActiveDropPopup->AddToViewport(DropPopupZOrder);
	ActiveDropPopup->ConfigureForItem(Item, Item->IsItemDroppable());
}

void UPlayerInventoryWidget::ClearActiveDropPopup()
{
	if (!ActiveDropPopup)
	{
		return;
	}

	UPlayerInventoryDropPopupWidget* PopupToClear = ActiveDropPopup;
	ActiveDropPopup = nullptr;

	PopupToClear->OnDropConfirmed.RemoveAll(this);
	PopupToClear->OnPopupClosed.RemoveAll(this);
	PopupToClear->RemoveFromParent();
}

void UPlayerInventoryWidget::HandleDropRequested(AHuntedInventoryItemBase* Item)
{
	ShowDropPopupForItem(Item);
}

void UPlayerInventoryWidget::HandleDropConfirmed(AHuntedInventoryItemBase* Item)
{
	if (!IsValid(CharacterReference))
	{
		return;
	}

	UPlayerInventoryComponent* InventoryComponent = CharacterReference->GetPlayerInventoryComponent();
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	if (InventoryComponent->DiscardItem(Item))
	{
		CharacterReference->ClearCachedItem();
	}
}

void UPlayerInventoryWidget::HandleDropPopupClosed()
{
	if (!ActiveDropPopup)
	{
		return;
	}

	ActiveDropPopup->OnDropConfirmed.RemoveAll(this);
	ActiveDropPopup->OnPopupClosed.RemoveAll(this);
	ActiveDropPopup = nullptr;
}

void UPlayerInventoryWidget::ShowDiscardWidgetForPickup(AHuntedInventoryItemBase* PendingPickup)
{
	if (!IsValid(PendingPickup) || !IsValid(CharacterReference))
	{
		return;
	}

	UPlayerInventoryComponent* InventoryComponent = CharacterReference->GetPlayerInventoryComponent();
	if (!IsValid(InventoryComponent) || !InventoryComponent->IsFullInventoryResolutionActive())
	{
		return;
	}

	if (!DiscardWidgetClass)
	{
		Debug::Print(TEXT("Player Inventory Widget: DiscardWidgetClass is not assigned"), FColor::Red);
		InventoryComponent->CancelFullInventoryResolution();
		return;
	}

	if (ActiveDiscardWidget)
	{
		ActiveDiscardWidget->OnResolutionClosed.RemoveAll(this);
		ActiveDiscardWidget->RemoveFromParent();
		ActiveDiscardWidget = nullptr;
	}

	bInventoryWasVisibleBeforeOverflow =
		GetVisibility() != ESlateVisibility::Collapsed && GetVisibility() != ESlateVisibility::Hidden;
	SetVisibility(ESlateVisibility::Visible);

	APlayerController* OwningPlayer = GetOwningPlayer();
	ActiveDiscardWidget = CreateWidget<UPlayerInventoryDiscardWidget>(OwningPlayer, DiscardWidgetClass);
	if (!ActiveDiscardWidget)
	{
		InventoryComponent->CancelFullInventoryResolution();
		RestoreInputAfterOverflow();
		return;
	}

	ActiveDiscardWidget->OnResolutionClosed.AddDynamic(
		this, &UPlayerInventoryWidget::HandleOverflowResolutionClosed);
	ActiveDiscardWidget->AddToViewport(DiscardWidgetZOrder);
	ActiveDiscardWidget->ConfigureForOverflow(InventoryComponent, PendingPickup);

	if (OwningPlayer)
	{
		bPreviousMouseCursorVisible = OwningPlayer->bShowMouseCursor;
		OwningPlayer->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(ActiveDiscardWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		OwningPlayer->SetInputMode(InputMode);
	}
}

void UPlayerInventoryWidget::RestoreInputAfterOverflow()
{
	if (!bInventoryWasVisibleBeforeOverflow)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}

	if (APlayerController* OwningPlayer = GetOwningPlayer())
	{
		OwningPlayer->bShowMouseCursor = bPreviousMouseCursorVisible;

		if (bInventoryWasVisibleBeforeOverflow)
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			OwningPlayer->SetInputMode(InputMode);
		}
		else
		{
			FInputModeGameOnly InputMode;
			OwningPlayer->SetInputMode(InputMode);
		}
	}
}

void UPlayerInventoryWidget::HandleInventoryOverflowRequested(AHuntedInventoryItemBase* PendingPickup)
{
	ShowDiscardWidgetForPickup(PendingPickup);
}

void UPlayerInventoryWidget::HandleOverflowResolutionClosed(bool bPickupAccepted)
{
	if (ActiveDiscardWidget)
	{
		ActiveDiscardWidget->OnResolutionClosed.RemoveAll(this);
		ActiveDiscardWidget = nullptr;
	}

	if (bPickupAccepted && IsValid(CharacterReference))
	{
		CharacterReference->ClearCachedItem();
	}

	RestoreInputAfterOverflow();
}
