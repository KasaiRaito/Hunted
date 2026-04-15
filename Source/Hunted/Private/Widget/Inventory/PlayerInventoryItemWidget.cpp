// KasaiRaito All Rights Reserved


#include "Widget/Inventory/PlayerInventoryItemWidget.h"

#include "Characters/HuntedPlayerCharacter.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Blueprint/DragDropOperation.h"

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
	AddItemWidget(CharacterReference->GetCachedItem());
}

void UPlayerInventoryItemWidget::AddItemWidget(AHuntedInventoryItemBase* ItemToAdd)
{
	if (!ItemToAdd)
	{
		Debug::Print("Plater Inventory Widget: No Item To Add");
		return;
	}
	Item = Cast<AHuntedInventoryItemBase>(ItemToAdd);
	
	ItemImage->SetBrushFromMaterial(ItemToAdd->GetIcon());
	
	Size = FVector2D( ItemToAdd->GetItemInventorySize() * CharacterReference->GetPlayerInventoryComponent()->GetTileSize());
	
	BackgroundSizeBox->SetWidthOverride(Size.X);
	BackgroundSizeBox->SetHeightOverride(Size.Y);
	
	UCanvasPanelSlot* ImageAsCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemImage);
	
	ImageAsCanvasSlot->SetSize(Size);
	
	// Change colors for feedback to the player
	FLinearColor BackgroundColor = FLinearColor(0.0f,0.0f,0.0f,0.5f);
	BackgroundBorder->SetBrushColor(BackgroundColor);
	FLinearColor ItemImageColor = FLinearColor(1.0f,1.0f,1.0f,0.5f);
	ItemImage->SetColorAndOpacity(ItemImageColor);
	//*
}

void UPlayerInventoryItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry,InMouseEvent);
	
	// Change colors for feedback to the player
	FLinearColor BackgroundColor = FLinearColor(0.0f,0.0f,0.0f,0.2f);
	BackgroundBorder->SetBrushColor(BackgroundColor);
	FLinearColor ItemImageColor = FLinearColor(1.0f,1.0f,1.0f,1.0f);
	ItemImage->SetColorAndOpacity(ItemImageColor);
	//*
}

void UPlayerInventoryItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	// Change colors for feedback to the player
	FLinearColor BackgroundColor = FLinearColor(0.0f,0.0f,0.0f,0.5f);
	BackgroundBorder->SetBrushColor(BackgroundColor);
	FLinearColor ItemImageColor = FLinearColor(1.0f,1.0f,1.0f,0.5f);
	ItemImage->SetColorAndOpacity(ItemImageColor);
	//*
}

void UPlayerInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	//Debug::Print("Plater Inventory Widget: Drag Detected", FColor::Red);
	
	// Change colors for feedback to the player
	FLinearColor BackgroundColor = FLinearColor(1.0f,1.0f,1.0f,0.5f);
	BackgroundBorder->SetBrushColor(BackgroundColor);
	FLinearColor ItemImageColor = FLinearColor(0.5f,0.5f,0.5f,1.0f);
	ItemImage->SetColorAndOpacity(ItemImageColor);
	
	//Set Variables for the Drag Ability
	UDragDropOperation* DragOperation = NewObject<UDragDropOperation>();
	DragOperation->DefaultDragVisual = this;
	DragOperation->Payload = Item;
	
	//Remove widget from inventory & Preview
	CharacterReference->GetPlayerInventoryComponent()->RemoveItem(Item);
	OutOperation = DragOperation;
	this-> RemoveFromParent();
}

FReply UPlayerInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
}
