// KasaiRaito All Rights Reserved


#include "Widget/Inventory/PlayerInventoryGridWidget.h"

#include "Characters/HuntedPlayerCharacter.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Widget/Inventory/PlayerInventoryItemWidget.h"
#include "HuntedDebugHelper.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"

void UPlayerInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetUpInventoryGrid();
	if (bGridDirty)
	{
		CreateLineSegments();
	}

	RefreshItemWidgets();
}

void UPlayerInventoryGridWidget::SetUpInventoryGrid()
{
	PlayerReference = Cast<AHuntedPlayerCharacter>(GetOwningPlayerPawn());
	
	if (!PlayerReference)
	{
		Debug::Print("[PlayerInventoryGrid] Can't construct player inventory component");
		return;
	}

	InventoryComponent = PlayerReference->GetPlayerInventoryComponent();
	if (!InventoryComponent)
	{
		Debug::Print("[PlayerInventoryGrid] Can't construct player inventory component");
		return;
	}
	
	InventoryComponent->SetInventoryGridWidget(this);
	
	Columns = InventoryComponent->GetColumns();
	Rows = InventoryComponent->GetRows();
	TileSize = InventoryComponent->GetTileSize();
	
	float NewWidth = Columns * TileSize;
	float NewHeight = Rows * TileSize;
	
	bGridDirty = Columns != CachedColumns
		|| Rows != CachedRows
		|| !FMath::IsNearlyEqual(TileSize, CachedTileSize);

	if (bGridDirty)
	{
		CachedColumns = Columns;
		CachedRows = Rows;
		CachedTileSize = TileSize;

		int32 LineCount = (Columns + 1) + (Rows + 1);
		LineStructData.XLines.Reset(LineCount);
		LineStructData.YLines.Reset(LineCount);
		StartX.Reset(LineCount);
		StartY.Reset(LineCount);
		EndX.Reset(LineCount);
		EndY.Reset(LineCount);
	}
	
	UCanvasPanelSlot* BorderAsCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
	if (BorderAsCanvasSlot)
	{
		BorderAsCanvasSlot->SetSize(FVector2D(NewWidth, NewHeight));
	}
}

void UPlayerInventoryGridWidget::CreateLineSegments()
{
	for (int8 Column = 0; Column <= Columns; Column++)
	{
		float X = Column * TileSize;
		
		LineStructData.XLines.Add(FVector2D(X, X));
		LineStructData.YLines.Add(FVector2D(0.0f, Rows * TileSize));
	}
	
	for (int8 Row = 0; Row <= Rows; Row++)
	{
		float Y = Row * TileSize;
		
		LineStructData.XLines.Add(FVector2D(0.0f, Columns * TileSize));
		LineStructData.YLines.Add(FVector2D(Y, Y));
	}
	
	for (const FVector2D Elements : LineStructData.XLines)
	{
		StartX.Add(Elements.X);
		EndX.Add(Elements.Y);
		
	}
	for (const FVector2D Elements : LineStructData.YLines)
	{
		StartY.Add(Elements.X);
		EndY.Add(Elements.Y);
	}

	bGridDirty = false;
}

int32 UPlayerInventoryGridWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const int32 SuperLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	
	FPaintContext PaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, SuperLayerId, InWidgetStyle, bParentEnabled);
	
	if (!GridBorder)
	{
		return SuperLayerId;
	}

	int32 DrawCount = FMath::Min(StartX.Num(), StartY.Num());
	DrawCount = FMath::Min(DrawCount, EndX.Num());
	DrawCount = FMath::Min(DrawCount, EndY.Num());
	if (DrawCount == 0)
	{
		return SuperLayerId;
	}

	FVector2D Offset = GridBorder->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.0f,0.0f));
	const FSlateBrush* WhiteBrush = FCoreStyle::Get().GetBrush(TEXT("WhiteBrush"));

	if (WhiteBrush && bDrawDraggedSourceTiles && DraggedSourceTiles.Num() > 0)
	{
		for (const FIntPoint& Tile : DraggedSourceTiles)
		{
			const FVector2D TilePosition = FVector2D(Tile.X * TileSize, Tile.Y * TileSize) + Offset;
			const FVector2D TileDimensions = FVector2D(TileSize, TileSize);
			PaintContext.MaxLayer++;

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				PaintContext.MaxLayer,
				AllottedGeometry.ToPaintGeometry(TileDimensions, FSlateLayoutTransform(TilePosition)),
				WhiteBrush,
				ESlateDrawEffect::None,
				DraggedSourceCellColor
			);
		}
	}

	if (WhiteBrush && bDrawDraggedTargetTiles && DraggedTargetTiles.Num() > 0)
	{
		for (const FIntPoint& Tile : DraggedTargetTiles)
		{
			const FVector2D TilePosition = FVector2D(Tile.X * TileSize, Tile.Y * TileSize) + Offset;
			const FVector2D TileDimensions = FVector2D(TileSize, TileSize);
			PaintContext.MaxLayer++;

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				PaintContext.MaxLayer,
				AllottedGeometry.ToPaintGeometry(TileDimensions, FSlateLayoutTransform(TilePosition)),
				WhiteBrush,
				ESlateDrawEffect::None,
				DraggedTargetTilesColor
			);
		}
	}
	
	for (int32 i = 0; i < DrawCount; i++)
	{
		UWidgetBlueprintLibrary::DrawLine(PaintContext, 
			FVector2D(StartX[i], StartY[i]) + Offset,
			FVector2D(EndX[i], EndY[i]) + Offset, CustomLineColor,
			false, InventoryLineThickness);
	}
		
	return PaintContext.MaxLayer;
}

bool UPlayerInventoryGridWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (!InOperation || !InOperation->Payload || !InventoryComponent)
	{
		return false;
	}
	
	AHuntedInventoryItemBase* DropedItem = Cast<AHuntedInventoryItemBase>(InOperation->Payload);
	
	if (!DropedItem)
	{
		return false;
	}

	const int8 TargetIndex = InventoryComponent->TileToIndex(DraggedItemTopLeftTile);

	bool bSourceConsumedByStacking = false;
	if (InventoryComponent->TryStackItemAtIndex(DropedItem, TargetIndex, bSourceConsumedByStacking))
	{
		SetDraggedItemVisualState(InOperation, EInventoryDragVisualState::ValidPlacement);
		ClearDraggedSourceTiles();
		ClearDraggedTargetTiles();

		if (bSourceConsumedByStacking)
		{
			InOperation->Tag = TEXT("DroppedToGrid");
			return true;
		}

		// Partial stack: the drag finish callback will place the residue back in its source cell.
		return true;
	}
	
	if (IsRoomAvailableFroPayload(DropedItem))
	{
		InventoryComponent->AddItemAtIndex(DropedItem, TargetIndex);
		InOperation->Tag = TEXT("DroppedToGrid");
		SetDraggedItemVisualState(InOperation, EInventoryDragVisualState::ValidPlacement);
		ClearDraggedSourceTiles();
		ClearDraggedTargetTiles();
		
		return true;
	}

	SetDraggedItemVisualState(InOperation, EInventoryDragVisualState::InvalidPlacement);
	ClearDraggedTargetTiles();
	
	return true;
}

bool UPlayerInventoryGridWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!InOperation || !InOperation->Payload || !InventoryComponent || !GridBorder || TileSize <= KINDA_SMALL_NUMBER)
	{
		return false;
	}
	
	AHuntedInventoryItemBase* DraggedItem = Cast<AHuntedInventoryItemBase>(InOperation->Payload);
	if (!DraggedItem)
	{
		return false;
	}
	
	FVector2D ScreenPosition = InDragDropEvent.GetScreenSpacePosition();
	FVector2D LocalPosition = InGeometry.AbsoluteToLocal(ScreenPosition);
	
	FVector2D GridStarterCoordinates = GridBorder->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.0f, 0.0f));
	FVector2D AdjustedLocalPosition = LocalPosition - GridStarterCoordinates;
	
	//Debug::Print(FString::Printf(TEXT("Dragged Screen: %.2f , %.2f"), ScreenPosition.X, ScreenPosition.Y));
	//Debug::Print(FString::Printf(TEXT("Dragged AdjustedLocal: %.2f , %.2f"), AdjustedLocalPosition.X, AdjustedLocalPosition.Y));
	
	FIntPoint CurrentTile; // Stores the tile to which the mouse is pointing
	const FMousePositionInTile MousePositionInTile = MousePositionInTileResult(AdjustedLocalPosition);
	const bool LocalHorizontal = MousePositionInTile.Horizontal;
	const bool LocalVertical = MousePositionInTile.Vertical;
	
	if (LocalHorizontal)
	{
		CurrentTile.X = FMath::Clamp(DraggedItem->GetItemInventorySize().X -1, 0, DraggedItem->GetItemInventorySize().X -1);
	}
	else
	{
		CurrentTile.X = FMath::Clamp(DraggedItem->GetItemInventorySize().X, 0, DraggedItem->GetItemInventorySize().X);
	}
	
	if (LocalVertical)
	{
		CurrentTile.Y = FMath::Clamp(DraggedItem->GetItemInventorySize().Y -1, 0, DraggedItem->GetItemInventorySize().Y -1);
	}
	else
	{
		CurrentTile.Y = FMath::Clamp(DraggedItem->GetItemInventorySize().Y, 0, DraggedItem->GetItemInventorySize().Y);
	}
	
	DraggedItemTopLeftTile = FIntPoint(FMath::TruncToInt(AdjustedLocalPosition.X / TileSize), 
		FMath::TruncToInt32(AdjustedLocalPosition.Y / TileSize)) - (CurrentTile / 2.0f);

	const bool bIsValidPlacement = IsRoomAvailableFroPayload(DraggedItem);
	UpdateDraggedTargetTiles(DraggedItem, bIsValidPlacement);
	SetDraggedItemVisualState(InOperation,
		bIsValidPlacement ? EInventoryDragVisualState::ValidPlacement : EInventoryDragVisualState::InvalidPlacement);
	
	return true;
}

void UPlayerInventoryGridWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	ClearDraggedTargetTiles();
	SetDraggedItemVisualState(InOperation, EInventoryDragVisualState::DropItem);
}

bool UPlayerInventoryGridWidget::IsRoomAvailableFroPayload(AHuntedInventoryItemBase* Item) const
{
	if (!Item)
	{
		return false;
	}
	
	return InventoryComponent->CanPlaceOrStackItemAtIndex(Item, InventoryComponent->TileToIndex(DraggedItemTopLeftTile));
}

FMousePositionInTile UPlayerInventoryGridWidget::MousePositionInTileResult(FVector2D MousePosition)
{
	if (!InventoryComponent)
	{
		CachedMousePositionInTile.Horizontal = false;
		CachedMousePositionInTile.Vertical = false;
		return CachedMousePositionInTile;
	}

	const float CurrentTileSize = InventoryComponent->GetTileSize();
	if (CurrentTileSize <= KINDA_SMALL_NUMBER)
	{
		CachedMousePositionInTile.Horizontal = false;
		CachedMousePositionInTile.Vertical = false;
		return CachedMousePositionInTile;
	}

	float HorizontalRemainder = FMath::Fmod(MousePosition.X, CurrentTileSize);
	float VerticalRemainder = FMath::Fmod(MousePosition.Y, CurrentTileSize);
	
	if (HorizontalRemainder < 0.0f)
	{
		HorizontalRemainder += CurrentTileSize;
	}
	if (VerticalRemainder < 0.0f)
	{
		VerticalRemainder += CurrentTileSize;
	}
	
	CachedMousePositionInTile.Horizontal = HorizontalRemainder >= (CurrentTileSize * 0.5f);
	CachedMousePositionInTile.Vertical = VerticalRemainder >= (CurrentTileSize * 0.5f);
	
	
	return CachedMousePositionInTile;
}

void UPlayerInventoryGridWidget::SetDraggedSourceTiles(const TArray<FIntPoint>& InTiles)
{
	DraggedSourceTiles = InTiles;
	bDrawDraggedSourceTiles = DraggedSourceTiles.Num() > 0;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void UPlayerInventoryGridWidget::ClearDraggedSourceTiles()
{
	if (!bDrawDraggedSourceTiles && DraggedSourceTiles.Num() == 0)
	{
		return;
	}
	
	DraggedSourceTiles.Reset();
	bDrawDraggedSourceTiles = false;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void UPlayerInventoryGridWidget::UpdateDraggedTargetTiles(AHuntedInventoryItemBase* DraggedItem, bool bIsValidPlacement)
{
	if (!DraggedItem)
	{
		ClearDraggedTargetTiles();
		return;
	}

	const FIntPoint ItemSize = DraggedItem->GetItemInventorySize();
	TArray<FIntPoint> TargetTiles;
	TargetTiles.Reserve(ItemSize.X * ItemSize.Y);

	for (int32 X = 0; X < ItemSize.X; X++)
	{
		for (int32 Y = 0; Y < ItemSize.Y; Y++)
		{
			const FIntPoint TileToHighlight = DraggedItemTopLeftTile + FIntPoint(X, Y);
			if (TileToHighlight.X >= 0 && TileToHighlight.Y >= 0
				&& TileToHighlight.X < Columns && TileToHighlight.Y < Rows)
			{
				TargetTiles.Add(TileToHighlight);
			}
		}
	}

	SetDraggedTargetTiles(TargetTiles, bIsValidPlacement ? DraggedTargetValidCellColor : DraggedTargetInvalidCellColor);
}

void UPlayerInventoryGridWidget::SetDraggedTargetTiles(const TArray<FIntPoint>& InTiles, const FLinearColor& InColor)
{
	DraggedTargetTiles = InTiles;
	DraggedTargetTilesColor = InColor;
	bDrawDraggedTargetTiles = DraggedTargetTiles.Num() > 0;
	Invalidate(EInvalidateWidgetReason::Paint);
}

void UPlayerInventoryGridWidget::SetDraggedItemVisualState(UDragDropOperation* InOperation,
	EInventoryDragVisualState NewState) const
{
	if (!InOperation)
	{
		return;
	}

	if (UPlayerInventoryItemWidget* DragVisualWidget = Cast<UPlayerInventoryItemWidget>(InOperation->DefaultDragVisual))
	{
		DragVisualWidget->SetDragVisualState(NewState);
	}
}

void UPlayerInventoryGridWidget::AddItemWidget()
{
	RefreshItemWidgets();
}

void UPlayerInventoryGridWidget::RefreshItemWidgets()
{
	if (!InventoryComponent || !GridCanvasPanel || !PlayerReference)
	{
		return;
	}

	GridCanvasPanel->ClearChildren();

	TMap<AHuntedInventoryItemBase*, FIntPoint> AllItems = InventoryComponent->GetAllItems();
	TArray<AHuntedInventoryItemBase*> ItemsInInventory;
	AllItems.GetKeys(ItemsInInventory);

	TSubclassOf<UUserWidget> ItemWidgetClass = PlayerReference->GetItemWidgetClass();
	if (!ItemWidgetClass)
	{
		return;
	}

	for (AHuntedInventoryItemBase* AddedItem : ItemsInInventory)
	{
		if (!AddedItem)
		{
			continue;
		}

		PlayerReference->SetCachedItem(AddedItem);
		UUserWidget* ItemWidget = CreateWidget<UUserWidget>(GetWorld(), ItemWidgetClass);
		if (!ItemWidget)
		{
			continue;
		}

		ItemWidget->SetOwningPlayer(GetOwningPlayer());
		if (UPlayerInventoryItemWidget* InventoryItemWidget = Cast<UPlayerInventoryItemWidget>(ItemWidget))
		{
			InventoryItemWidget->InitializeInventoryItem(AddedItem);
		}

		PanelSlot = GridCanvasPanel->AddChild(ItemWidget);
		UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(PanelSlot);
		if (!CanvasPanelSlot)
		{
			continue;
		}

		const FIntPoint& ItemTile = AllItems[AddedItem];
		CanvasPanelSlot->SetAutoSize(true);
		CanvasPanelSlot->SetPosition(FVector2D(ItemTile.X * TileSize, ItemTile.Y * TileSize));
	}
}

void UPlayerInventoryGridWidget::ClearDraggedTargetTiles()
{
	if (!bDrawDraggedTargetTiles && DraggedTargetTiles.Num() == 0)
	{
		return;
	}

	DraggedTargetTiles.Reset();
	DraggedTargetTilesColor = FLinearColor::Transparent;
	bDrawDraggedTargetTiles = false;
	Invalidate(EInvalidateWidgetReason::Paint);
}
