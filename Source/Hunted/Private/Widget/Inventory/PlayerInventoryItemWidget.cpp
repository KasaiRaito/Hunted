// KasaiRaito All Rights Reserved


#include "Widget/Inventory/PlayerInventoryItemWidget.h"

#include "Characters/HuntedPlayerCharacter.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/DragDropOperation.h"
#include "Widget/Inventory/PlayerInventoryGridWidget.h"
#include "Engine/Texture2D.h"
#include "InputCoreTypes.h"

#include "HuntedDebugHelper.h"

namespace
{
	TWeakObjectPtr<UPlayerInventoryItemWidget> ActiveInventoryContextMenuOwner;
	constexpr int32 InventoryContextMenuLayerZOrder = 10000;
}

void UPlayerInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CharacterReference = Cast<AHuntedPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!CharacterReference)
	{
		Debug::Print("Plater Inventory Widget Couldn't Get: Player Character Reference");
		return;
	}
	if (Item)
	{
		AddItemWidget(Item);
		return;
	}

	AddItemWidget(CharacterReference->GetCachedItem());
}

void UPlayerInventoryItemWidget::NativeDestruct()
{
	HideContextMenu();
	Super::NativeDestruct();
}

void UPlayerInventoryItemWidget::AddItemWidget(AHuntedInventoryItemBase* ItemToAdd)
{
	if (!ItemToAdd)
	{
		Debug::Print("Plater Inventory Widget: No Item To Add");
		return;
	}

	if (!CharacterReference || !CharacterReference->GetPlayerInventoryComponent() || !ItemImage || !BackgroundSizeBox)
	{
		return;
	}

	Item = Cast<AHuntedInventoryItemBase>(ItemToAdd);
	
	ItemImage->SetBrushFromMaterial(ItemToAdd->GetIcon());
	
	Size = FVector2D( ItemToAdd->GetItemInventorySize() * CharacterReference->GetPlayerInventoryComponent()->GetTileSize());
	
	BackgroundSizeBox->SetWidthOverride(Size.X);
	BackgroundSizeBox->SetHeightOverride(Size.Y);
	
	if (UCanvasPanelSlot* ImageAsCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemImage))
	{
		ImageAsCanvasSlot->SetSize(Size);
	}

	EnsureStackCounterWidget();
	EnsureContextMenuWidget();
	RebuildContextMenuEntries();
	UpdateStackCounterVisual();
	RefreshCombineVisualState();
}

void UPlayerInventoryItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry,InMouseEvent);

	if (bIsDragging)
	{
		return;
	}

	if (CharacterReference && CharacterReference->GetPlayerInventoryComponent()
		&& CharacterReference->GetPlayerInventoryComponent()->IsCombineModeActive())
	{
		RefreshCombineVisualState();
		return;
	}

	ApplyBackgroundAndImageColors(HoverBackgroundColor, HoverImageColor);
}

void UPlayerInventoryItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (bIsDragging)
	{
		return;
	}

	if (CharacterReference && CharacterReference->GetPlayerInventoryComponent()
		&& CharacterReference->GetPlayerInventoryComponent()->IsCombineModeActive())
	{
		RefreshCombineVisualState();
		return;
	}

	ApplyBackgroundAndImageColors(IdleBackgroundColor, IdleImageColor);
}

void UPlayerInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	//Debug::Print("Plater Inventory Widget: Drag Detected", FColor::Red);

	HideContextMenu();
	
	bIsDragging = true;
	SetDragVisualState(EInventoryDragVisualState::InvalidPlacement);
	
	//Set Variables for the Drag Ability
	UPlayerInventoryDragDropOperation* DragOperation = NewObject<UPlayerInventoryDragDropOperation>();
	DragOperation->DefaultDragVisual = this;
	DragOperation->Payload = Item;

	if (DragOperation)
	{
		DragOperation->OnDrop.AddDynamic(this, &UPlayerInventoryItemWidget::HandleDragOperationFinished);
		DragOperation->OnDragCancelled.AddDynamic(this, &UPlayerInventoryItemWidget::HandleDragOperationFinished);
	}
	
	// Remove the widget from the grid while keeping the inventory data intact until drop resolution.
	if (UPlayerInventoryComponent* InventoryComponent = CharacterReference->GetPlayerInventoryComponent())
	{
		bHasDragStartTile = InventoryComponent->FindItemTopLeftTile(Item, DragStartTopLeftTile);
		
		if (UPlayerInventoryGridWidget* InventoryGrid = InventoryComponent->GetPlayerInventoryGridWidget())
		{
			if (bHasDragStartTile)
			{
				const FIntPoint ItemSize = Item->GetItemInventorySize();
				TArray<FIntPoint> SourceTiles;
				SourceTiles.Reserve(ItemSize.X * ItemSize.Y);

				for (int32 X = 0; X < ItemSize.X; X++)
				{
					for (int32 Y = 0; Y < ItemSize.Y; Y++)
					{
						SourceTiles.Add(FIntPoint(DragStartTopLeftTile.X + X, DragStartTopLeftTile.Y + Y));
					}
				}

				InventoryGrid->SetDraggedSourceTiles(SourceTiles);
			}
			else
			{
				InventoryGrid->ClearDraggedSourceTiles();
			}
		}
	}

	DragOperation->InitializeInventoryDrag(Item, bHasDragStartTile, DragStartTopLeftTile);
	
	OutOperation = DragOperation;
	
	this-> RemoveFromParent();
}

FReply UPlayerInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	if (!CharacterReference || !CharacterReference->GetPlayerInventoryComponent() || !Item)
	{
		return FReply::Unhandled();
	}

	UPlayerInventoryComponent* InventoryComponent = CharacterReference->GetPlayerInventoryComponent();
	if (ActiveInventoryContextMenuOwner.IsValid() && ActiveInventoryContextMenuOwner.Get() != this)
	{
		ActiveInventoryContextMenuOwner->HideContextMenu();
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		ToggleContextMenu();
		return FReply::Handled();
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && InventoryComponent->IsCombineModeActive())
	{
		HideContextMenu();

		if (InventoryComponent->GetPendingCombineItem() == Item)
		{
			InventoryComponent->CancelCombineSelection();
			return FReply::Handled();
		}

		InventoryComponent->TryCombineItems(InventoryComponent->GetPendingCombineItem(), Item);
		return FReply::Handled();
	}

	return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
}

void UPlayerInventoryItemWidget::HandleDragOperationFinished(UDragDropOperation* Operation)
{
	if (!CharacterReference)
	{
		return;
	}

	bIsDragging = false;

	if (!Operation)
	{
		SetDragVisualState(EInventoryDragVisualState::Idle);
		return;
	}

	const bool bDropResolved = Operation->Tag == TEXT("DroppedToGrid")
		|| Operation->Tag == TEXT("DroppedToWorld")
		|| Operation->Tag == TEXT("RemovedFromInventory");
	if (!bDropResolved)
	{
		if (UPlayerInventoryDragDropOperation* InventoryDragOperation = Cast<UPlayerInventoryDragDropOperation>(Operation))
		{
			InventoryDragOperation->RestoreOriginalItemSize();
		}
	}

	if (UPlayerInventoryComponent* InventoryComponent = CharacterReference->GetPlayerInventoryComponent())
	{
		if (UPlayerInventoryGridWidget* InventoryGrid = InventoryComponent->GetPlayerInventoryGridWidget())
		{
			InventoryGrid->ClearDraggedSourceTiles();
			InventoryGrid->ClearDraggedTargetTiles();
			InventoryGrid->RefreshItemWidgets();
		}
	}

	SetDragVisualState(EInventoryDragVisualState::Idle);
	bHasDragStartTile = false;
	DragStartTopLeftTile = FIntPoint::ZeroValue;
	RefreshCombineVisualState();
}

void UPlayerInventoryItemWidget::ApplyBackgroundAndImageColors(const FLinearColor& InBackgroundColor,
	const FLinearColor& InImageColor)
{
	if (BackgroundBorder)
	{
		BackgroundBorder->SetBrushColor(InBackgroundColor);
	}

	if (ItemImage)
	{
		ItemImage->SetColorAndOpacity(InImageColor);
	}
}

void UPlayerInventoryItemWidget::EnsureStackCounterWidget()
{
	if (StackCounterText || !CanvasPanel || !WidgetTree)
	{
		return;
	}

	StackCounterText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StackCounterText_Auto"));
	if (!StackCounterText)
	{
		return;
	}

	StackCounterText->SetColorAndOpacity(StackCounterColor);
	StackCounterText->SetJustification(ETextJustify::Right);
	StackCounterText->SetVisibility(ESlateVisibility::Collapsed);

	CanvasPanel->AddChild(StackCounterText);
	if (UCanvasPanelSlot* TextSlot = Cast<UCanvasPanelSlot>(StackCounterText->Slot))
	{
		TextSlot->SetAutoSize(true);
		TextSlot->SetAnchors(FAnchors(1.0f, 1.0f));
		TextSlot->SetAlignment(FVector2D(1.0f, 1.0f));
		TextSlot->SetPosition(FVector2D(-4.0f, -2.0f));
		TextSlot->SetZOrder(100);
	}
}

void UPlayerInventoryItemWidget::UpdateStackCounterVisual()
{
	if (!StackCounterText)
	{
		return;
	}

	if (!Item)
	{
		StackCounterText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	const FHuntedPlayerItemData ItemData = Item->GetItemData();
	if (!ItemData.IsStackable)
	{
		StackCounterText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	const int32 StackAmount = FMath::Max(1, ItemData.ItemAmount);
	StackCounterText->SetText(FText::AsNumber(StackAmount));
	StackCounterText->SetColorAndOpacity(StackCounterColor);
	StackCounterText->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPlayerInventoryItemWidget::EnsureContextMenuWidget()
{
	if (ContextMenuBorder || !CanvasPanel || !WidgetTree)
	{
		return;
	}

	ContextMenuBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("InventoryContextMenuBorder"));
	ContextMenuBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("InventoryContextMenuBox"));
	if (!ContextMenuBorder || !ContextMenuBox)
	{
		return;
	}

	ContextMenuBorder->SetBrushColor(FLinearColor(0.05f, 0.05f, 0.05f, 0.95f));
	ContextMenuBorder->SetPadding(ContextMenuPadding);
	ContextMenuBorder->SetVisibility(ESlateVisibility::Collapsed);
	ContextMenuBorder->SetContent(ContextMenuBox);

	CanvasPanel->AddChild(ContextMenuBorder);
	if (UCanvasPanelSlot* MenuSlot = Cast<UCanvasPanelSlot>(ContextMenuBorder->Slot))
	{
		MenuSlot->SetAutoSize(true);
		MenuSlot->SetAnchors(FAnchors(1.0f, 0.5f));
		MenuSlot->SetAlignment(FVector2D(0.0f, 0.5f));
		MenuSlot->SetPosition(ContextMenuOffset);
		MenuSlot->SetZOrder(InventoryContextMenuLayerZOrder);
	}
}

void UPlayerInventoryItemWidget::RebuildContextMenuEntries()
{
	if (!ContextMenuBox || !WidgetTree)
	{
		return;
	}

	ContextMenuActions.Reset();
	ContextMenuActions.Add(MakeContextActionEntry(EInventoryContextAction::Inspect, FText::FromString(TEXT("Inspect")), InspectActionIcon));
	ContextMenuActions.Add(MakeContextActionEntry(EInventoryContextAction::Combine, FText::FromString(TEXT("Combine")), CombineActionIcon));
	ContextMenuActions.Add(MakeContextActionEntry(EInventoryContextAction::Discard, FText::FromString(TEXT("Discard")), DiscardActionIcon));

	ContextMenuBox->ClearChildren();

	for (const FHuntedInventoryContextActionEntry& ActionEntry : ContextMenuActions)
	{
		UButton* ActionButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
		UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		if (!ActionButton || !Row)
		{
			continue;
		}

		ActionButton->SetContent(Row);

		UImage* IconImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		if (IconImage)
		{
			if (ActionEntry.Icon)
			{
				IconImage->SetBrushFromTexture(ActionEntry.Icon);
				IconImage->SetDesiredSizeOverride(FVector2D(ContextMenuIconSize, ContextMenuIconSize));
				IconImage->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			else
			{
				IconImage->SetVisibility(ESlateVisibility::Collapsed);
			}

			Row->AddChildToHorizontalBox(IconImage);
			if (UHorizontalBoxSlot* IconSlot = Cast<UHorizontalBoxSlot>(IconImage->Slot))
			{
				IconSlot->SetPadding(FMargin(0.0f, 0.0f, 5.0f, 0.0f));
				IconSlot->SetVerticalAlignment(VAlign_Center);
				IconSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
			}
		}

		UTextBlock* LabelText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		if (LabelText)
		{
			LabelText->SetText(ActionEntry.Label);
			LabelText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
			LabelText->SetFont(FSlateFontInfo(LabelText->GetFont().FontObject, ContextMenuFontSize, LabelText->GetFont().TypefaceFontName));
			LabelText->SetVisibility(ESlateVisibility::HitTestInvisible);
			Row->AddChildToHorizontalBox(LabelText);
		}

		switch (ActionEntry.Action)
		{
		case EInventoryContextAction::Inspect:
			ActionButton->OnClicked.AddDynamic(this, &UPlayerInventoryItemWidget::HandleInspectClicked);
			break;
		case EInventoryContextAction::Combine:
			ActionButton->OnClicked.AddDynamic(this, &UPlayerInventoryItemWidget::HandleCombineClicked);
			break;
		case EInventoryContextAction::Discard:
			ActionButton->OnClicked.AddDynamic(this, &UPlayerInventoryItemWidget::HandleDiscardClicked);
			break;
		default:
			break;
		}

		ContextMenuBox->AddChildToVerticalBox(ActionButton);
		if (UVerticalBoxSlot* ButtonSlot = Cast<UVerticalBoxSlot>(ActionButton->Slot))
		{
			ButtonSlot->SetPadding(ContextMenuEntryPadding);
		}
	}
}

void UPlayerInventoryItemWidget::ToggleContextMenu()
{
	EnsureContextMenuWidget();
	RebuildContextMenuEntries();

	if (!ContextMenuBorder)
	{
		return;
	}

	if (IsContextMenuVisible())
	{
		HideContextMenu();
		return;
	}

	if (ActiveInventoryContextMenuOwner.IsValid() && ActiveInventoryContextMenuOwner.Get() != this)
	{
		ActiveInventoryContextMenuOwner->HideContextMenu();
	}

	ActiveInventoryContextMenuOwner = this;
	RaiseContextMenuLayer();
	ContextMenuBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UPlayerInventoryItemWidget::HideContextMenu()
{
	if (ContextMenuBorder)
	{
		ContextMenuBorder->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ActiveInventoryContextMenuOwner.Get() == this)
	{
		ActiveInventoryContextMenuOwner.Reset();
	}

	RestoreContextMenuLayer();
}

bool UPlayerInventoryItemWidget::IsContextMenuVisible() const
{
	return ContextMenuBorder && ContextMenuBorder->GetVisibility() != ESlateVisibility::Collapsed;
}

bool UPlayerInventoryItemWidget::IsAnyContextMenuOpen()
{
	return ActiveInventoryContextMenuOwner.IsValid()
		&& ActiveInventoryContextMenuOwner->IsContextMenuVisible();
}

void UPlayerInventoryItemWidget::RaiseContextMenuLayer()
{
	if (CharacterReference && CharacterReference->GetPlayerInventoryComponent())
	{
		if (UPlayerInventoryGridWidget* InventoryGrid = CharacterReference->GetPlayerInventoryComponent()->GetPlayerInventoryGridWidget())
		{
			if (UCanvasPanelSlot* GridCanvasSlot = Cast<UCanvasPanelSlot>(InventoryGrid->Slot))
			{
				if (!bHasCachedInventoryGridSlotZOrder)
				{
					CachedInventoryGridSlotZOrder = GridCanvasSlot->GetZOrder();
					bHasCachedInventoryGridSlotZOrder = true;
				}

				GridCanvasSlot->SetZOrder(InventoryContextMenuLayerZOrder);
			}
		}
	}

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		if (!bHasCachedCanvasSlotZOrder)
		{
			CachedCanvasSlotZOrder = CanvasSlot->GetZOrder();
			bHasCachedCanvasSlotZOrder = true;
		}

		CanvasSlot->SetZOrder(InventoryContextMenuLayerZOrder);
	}

	if (ContextMenuBorder)
	{
		if (UCanvasPanelSlot* ContextMenuSlot = Cast<UCanvasPanelSlot>(ContextMenuBorder->Slot))
		{
			ContextMenuSlot->SetZOrder(InventoryContextMenuLayerZOrder);
		}
	}
}

void UPlayerInventoryItemWidget::RestoreContextMenuLayer()
{
	if (bHasCachedInventoryGridSlotZOrder && CharacterReference && CharacterReference->GetPlayerInventoryComponent())
	{
		if (UPlayerInventoryGridWidget* InventoryGrid = CharacterReference->GetPlayerInventoryComponent()->GetPlayerInventoryGridWidget())
		{
			if (UCanvasPanelSlot* GridCanvasSlot = Cast<UCanvasPanelSlot>(InventoryGrid->Slot))
			{
				GridCanvasSlot->SetZOrder(CachedInventoryGridSlotZOrder);
			}
		}
	}

	bHasCachedInventoryGridSlotZOrder = false;

	if (!bHasCachedCanvasSlotZOrder)
	{
		return;
	}

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		CanvasSlot->SetZOrder(CachedCanvasSlotZOrder);
	}

	bHasCachedCanvasSlotZOrder = false;
}

void UPlayerInventoryItemWidget::RefreshCombineVisualState()
{
	if (!CharacterReference || !CharacterReference->GetPlayerInventoryComponent() || !Item)
	{
		SetDragVisualState(EInventoryDragVisualState::Idle);
		return;
	}

	UPlayerInventoryComponent* InventoryComponent = CharacterReference->GetPlayerInventoryComponent();
	if (!InventoryComponent->IsCombineModeActive())
	{
		ApplyBackgroundAndImageColors(IdleBackgroundColor, IdleImageColor);
		return;
	}

	const bool bIsSelectedSource = InventoryComponent->GetPendingCombineItem() == Item;
	const bool bCanCombine = InventoryComponent->CanItemCombineWithPendingSelection(Item);
	ApplyCombineVisualState(bIsSelectedSource, bCanCombine);
}

void UPlayerInventoryItemWidget::ApplyCombineVisualState(bool bIsSelectedSource, bool bCanCombine)
{
	if (bIsSelectedSource)
	{
		ApplyBackgroundAndImageColors(CombineSelectedBackgroundColor, CombineSelectedImageColor);
		return;
	}

	if (bCanCombine)
	{
		ApplyBackgroundAndImageColors(CombineCompatibleBackgroundColor, CombineCompatibleImageColor);
		return;
	}

	ApplyBackgroundAndImageColors(CombineIncompatibleBackgroundColor, CombineIncompatibleImageColor);
}

FHuntedInventoryContextActionEntry UPlayerInventoryItemWidget::MakeContextActionEntry(EInventoryContextAction Action,
	const FText& Label, UTexture2D* Icon) const
{
	FHuntedInventoryContextActionEntry Entry;
	Entry.Action = Action;
	Entry.Label = Label;
	Entry.Icon = Icon;
	return Entry;
}

void UPlayerInventoryItemWidget::HandleInspectClicked()
{
	HideContextMenu();
	BP_OnInspectRequested(Item);
}

void UPlayerInventoryItemWidget::HandleCombineClicked()
{
	HideContextMenu();

	if (!CharacterReference || !CharacterReference->GetPlayerInventoryComponent() || !Item)
	{
		return;
	}

	CharacterReference->GetPlayerInventoryComponent()->BeginCombineSelection(Item);
}

void UPlayerInventoryItemWidget::HandleDiscardClicked()
{
	HideContextMenu();

	if (!CharacterReference || !CharacterReference->GetPlayerInventoryComponent() || !Item)
	{
		return;
	}

	CharacterReference->GetPlayerInventoryComponent()->DiscardItem(Item);
}

void UPlayerInventoryItemWidget::InitializeInventoryItem(AHuntedInventoryItemBase* ItemToAdd)
{
	Item = ItemToAdd;

	if (CharacterReference && Item)
	{
		AddItemWidget(Item);
	}
}

void UPlayerInventoryItemWidget::RefreshItemVisualLayout()
{
	if (Item)
	{
		AddItemWidget(Item);
	}
}

void UPlayerInventoryItemWidget::SetDragVisualState(EInventoryDragVisualState NewState)
{
	switch (NewState)
	{
	case EInventoryDragVisualState::Idle:
		ApplyBackgroundAndImageColors(IdleBackgroundColor, IdleImageColor);
		break;
	case EInventoryDragVisualState::ValidPlacement:
		ApplyBackgroundAndImageColors(ValidPlacementBackgroundColor, DragImageColor);
		break;
	case EInventoryDragVisualState::InvalidPlacement:
		ApplyBackgroundAndImageColors(InvalidPlacementBackgroundColor, DragImageColor);
		break;
	case EInventoryDragVisualState::DropItem:
		ApplyBackgroundAndImageColors(DropItemBackgroundColor, DragImageColor);
		break;
	default:
		ApplyBackgroundAndImageColors(IdleBackgroundColor, IdleImageColor);
		break;
	}
}

void UPlayerInventoryDragDropOperation::InitializeInventoryDrag(AHuntedInventoryItemBase* InDraggedItem,
	bool bInHasDragStartTile, FIntPoint InDragStartTopLeftTile)
{
	DraggedItem = InDraggedItem;
	bHasDragStartTile = bInHasDragStartTile;
	DragStartTopLeftTile = InDragStartTopLeftTile;
	OriginalItemSize = DraggedItem ? DraggedItem->GetItemInventorySize() : FIntPoint::ZeroValue;
	bWasRightMouseButtonDown = false;
	bIsRotated = false;
}

bool UPlayerInventoryDragDropOperation::ToggleDraggedItemRotation()
{
	if (!DraggedItem)
	{
		return false;
	}

	if (OriginalItemSize.X <= 0 || OriginalItemSize.Y <= 0 || OriginalItemSize.X == OriginalItemSize.Y)
	{
		return false;
	}

	const FIntPoint NewSize = bIsRotated
		? OriginalItemSize
		: FIntPoint(OriginalItemSize.Y, OriginalItemSize.X);

	DraggedItem->SetItemInventorySize(NewSize);
	bIsRotated = !bIsRotated;

	if (UPlayerInventoryItemWidget* DragVisualWidget = Cast<UPlayerInventoryItemWidget>(DefaultDragVisual))
	{
		DragVisualWidget->RefreshItemVisualLayout();
	}

	return true;
}

void UPlayerInventoryDragDropOperation::RestoreOriginalItemSize()
{
	if (!DraggedItem || OriginalItemSize.X <= 0 || OriginalItemSize.Y <= 0)
	{
		return;
	}

	DraggedItem->SetItemInventorySize(OriginalItemSize);
	bIsRotated = false;

	if (UPlayerInventoryItemWidget* DragVisualWidget = Cast<UPlayerInventoryItemWidget>(DefaultDragVisual))
	{
		DragVisualWidget->RefreshItemVisualLayout();
	}
}

void UPlayerInventoryDragDropOperation::HandleRotationInput(const FPointerEvent& PointerEvent)
{
	const bool bIsRightMouseButtonDown = PointerEvent.IsMouseButtonDown(EKeys::RightMouseButton);
	if (bIsRightMouseButtonDown && !bWasRightMouseButtonDown)
	{
		ToggleDraggedItemRotation();
	}

	bWasRightMouseButtonDown = bIsRightMouseButtonDown;
}

void UPlayerInventoryDragDropOperation::Dragged_Implementation(const FPointerEvent& PointerEvent)
{
	Super::Dragged_Implementation(PointerEvent);
	HandleRotationInput(PointerEvent);
}
