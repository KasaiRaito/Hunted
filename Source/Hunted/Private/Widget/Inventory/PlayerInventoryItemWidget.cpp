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
#include "Blueprint/DragDropOperation.h"
#include "Widget/Inventory/PlayerInventoryGridWidget.h"

#include "HuntedDebugHelper.h"

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
	UpdateStackCounterVisual();
	
	SetDragVisualState(EInventoryDragVisualState::Idle);
}

void UPlayerInventoryItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry,InMouseEvent);

	if (bIsDragging)
	{
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
	
	ApplyBackgroundAndImageColors(IdleBackgroundColor, IdleImageColor);
}

void UPlayerInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	//Debug::Print("Plater Inventory Widget: Drag Detected", FColor::Red);
	
	bIsDragging = true;
	SetDragVisualState(EInventoryDragVisualState::InvalidPlacement);
	
	//Set Variables for the Drag Ability
	UDragDropOperation* DragOperation = NewObject<UDragDropOperation>();
	DragOperation->DefaultDragVisual = this;
	DragOperation->Payload = Item;

	if (DragOperation)
	{
		DragOperation->OnDrop.AddDynamic(this, &UPlayerInventoryItemWidget::HandleDragOperationFinished);
		DragOperation->OnDragCancelled.AddDynamic(this, &UPlayerInventoryItemWidget::HandleDragOperationFinished);
	}
	
	//Remove widget from inventory & Preview
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
		
		InventoryComponent->RemoveItem(Item);
	}
	
	OutOperation = DragOperation;
	
	this-> RemoveFromParent();
}

FReply UPlayerInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
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

	if (UPlayerInventoryComponent* InventoryComponent = CharacterReference->GetPlayerInventoryComponent())
	{
		const bool bDroppedToWorld = Operation->Tag.Equals(TEXT("DroppedToWorld"));
		const bool bRemovedFromInventory = Operation->Tag.Equals(TEXT("RemovedFromInventory"));

		FIntPoint ExistingTile;
		const bool bItemAlreadyInInventory = Item && InventoryComponent->FindItemTopLeftTile(Item, ExistingTile);
		if (!bItemAlreadyInInventory && bHasDragStartTile && !bDroppedToWorld && !bRemovedFromInventory)
		{
			InventoryComponent->AddItemAtIndex(Item, InventoryComponent->TileToIndex(DragStartTopLeftTile));
		}

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

void UPlayerInventoryItemWidget::InitializeInventoryItem(AHuntedInventoryItemBase* ItemToAdd)
{
	Item = ItemToAdd;

	if (CharacterReference && Item)
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
