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
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UPlayerInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetUpInventoryGrid();
	if (bGridDirty)
	{
		CreateLineSegments();
	}

	EnsureItemInfoPanel();
	UpdateItemInfoPanelLayout();
	RefreshItemInfoPanel();
	RefreshItemWidgets();
}

void UPlayerInventoryGridWidget::SetUpInventoryGrid()
{
	PlayerReference = Cast<AHuntedPlayerCharacter>(GetOwningPlayerPawn());
	
	if (!IsValid(PlayerReference))
	{
		Debug::Print("[PlayerInventoryGrid] Can't construct player inventory component");
		return;
	}

	InventoryComponent = PlayerReference->GetPlayerInventoryComponent();
	if (!IsValid(InventoryComponent))
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

	EnsureItemInfoPanel();
	UpdateItemInfoPanelLayout();
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

	if (UPlayerInventoryItemWidget::IsAnyContextMenuOpen())
	{
		return SuperLayerId;
	}

	FPaintContext PaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, SuperLayerId, InWidgetStyle, bParentEnabled);
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
	if (!InOperation || !IsValid(InOperation->Payload) || !IsValid(InventoryComponent))
	{
		// Inventory widgets can refresh while a drag is active; reject stale payloads instead of moving them.
		return false;
	}
	
	AHuntedInventoryItemBase* DropedItem = Cast<AHuntedInventoryItemBase>(InOperation->Payload);
	
	if (!IsValid(DropedItem))
	{
		return false;
	}

	const int8 TargetIndex = InventoryComponent->TileToIndex(DraggedItemTopLeftTile);

	bool bSourceConsumed = false;
	if (InventoryComponent->TryMoveItemToIndex(DropedItem, TargetIndex, bSourceConsumed))
	{
		InOperation->Tag = TEXT("DroppedToGrid");
		SetDraggedItemVisualState(InOperation, EInventoryDragVisualState::ValidPlacement);
		ClearDraggedSourceTiles();
		ClearDraggedTargetTiles();

		if (bSourceConsumed)
		{
			// Full stack merges consume the dragged source item entirely.
		}

		return true;
	}

	SetDraggedItemVisualState(InOperation, EInventoryDragVisualState::InvalidPlacement);
	ClearDraggedTargetTiles();
	
	return true;
}

bool UPlayerInventoryGridWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!InOperation || !IsValid(InOperation->Payload) || !IsValid(InventoryComponent) || !GridBorder || TileSize <= KINDA_SMALL_NUMBER)
	{
		return false;
	}
	
	AHuntedInventoryItemBase* DraggedItem = Cast<AHuntedInventoryItemBase>(InOperation->Payload);
	if (!IsValid(DraggedItem))
	{
		return false;
	}

	if (UPlayerInventoryDragDropOperation* InventoryDragOperation = Cast<UPlayerInventoryDragDropOperation>(InOperation))
	{
		InventoryDragOperation->HandleRotationInput(InDragDropEvent);
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
	if (!IsValid(Item) || !IsValid(InventoryComponent))
	{
		return false;
	}
	
	return InventoryComponent->CanMoveItemToIndex(Item, InventoryComponent->TileToIndex(DraggedItemTopLeftTile));
}

FMousePositionInTile UPlayerInventoryGridWidget::MousePositionInTileResult(FVector2D MousePosition)
{
	if (!IsValid(InventoryComponent))
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

void UPlayerInventoryGridWidget::EnsureItemInfoPanel()
{
	if (ItemInfoBorder && ItemInfoTitleText && ItemInfoDescriptionText)
	{
		ItemInfoBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemInfoTitleText->SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemInfoDescriptionText->SetVisibility(ESlateVisibility::HitTestInvisible);
		return;
	}

	if (!CanvasPanel || !WidgetTree)
	{
		return;
	}

	bUsesGeneratedItemInfoPanel = true;
	ItemInfoBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("InventoryItemInfoBorder_Auto"));
	ItemInfoBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("InventoryItemInfoBox_Auto"));
	ItemInfoTitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("InventoryItemInfoTitleText_Auto"));
	ItemInfoDescriptionText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("InventoryItemInfoDescriptionText_Auto"));
	if (!ItemInfoBorder || !ItemInfoBox || !ItemInfoTitleText || !ItemInfoDescriptionText)
	{
		return;
	}

	ItemInfoBorder->SetBrushColor(ItemInfoBackgroundColor);
	ItemInfoBorder->SetPadding(ItemInfoContentPadding);
	ItemInfoBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
	ItemInfoBorder->SetContent(ItemInfoBox);

	ItemInfoTitleText->SetColorAndOpacity(FSlateColor(ItemInfoTitleColor));
	ItemInfoTitleText->SetAutoWrapText(false);
	ItemInfoTitleText->SetVisibility(ESlateVisibility::HitTestInvisible);
	FSlateFontInfo TitleFont = ItemInfoTitleText->GetFont();
	TitleFont.Size = FMath::RoundToInt(ItemInfoTitleFontSize);
	ItemInfoTitleText->SetFont(TitleFont);

	ItemInfoDescriptionText->SetColorAndOpacity(FSlateColor(ItemInfoDescriptionColor));
	ItemInfoDescriptionText->SetAutoWrapText(true);
	ItemInfoDescriptionText->SetVisibility(ESlateVisibility::HitTestInvisible);
	FSlateFontInfo DescriptionFont = ItemInfoDescriptionText->GetFont();
	DescriptionFont.Size = FMath::RoundToInt(ItemInfoDescriptionFontSize);
	ItemInfoDescriptionText->SetFont(DescriptionFont);

	ItemInfoBox->AddChildToVerticalBox(ItemInfoTitleText);
	if (UVerticalBoxSlot* TitleSlot = Cast<UVerticalBoxSlot>(ItemInfoTitleText->Slot))
	{
		TitleSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));
		TitleSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
	}

	ItemInfoBox->AddChildToVerticalBox(ItemInfoDescriptionText);
	if (UVerticalBoxSlot* DescriptionSlot = Cast<UVerticalBoxSlot>(ItemInfoDescriptionText->Slot))
	{
		DescriptionSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}

	CanvasPanel->AddChild(ItemInfoBorder);
}

void UPlayerInventoryGridWidget::UpdateItemInfoPanelLayout()
{
	if (!ItemInfoBorder || !GridBorder)
	{
		return;
	}

	UCanvasPanelSlot* InfoSlot = Cast<UCanvasPanelSlot>(ItemInfoBorder->Slot);
	if (!InfoSlot)
	{
		return;
	}

	FVector2D GridPosition = FVector2D::ZeroVector;
	FVector2D GridSize = FVector2D(Columns * TileSize, Rows * TileSize);
	if (UCanvasPanelSlot* GridSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder))
	{
		GridPosition = GridSlot->GetPosition();
		const FVector2D SlotSize = GridSlot->GetSize();
		if (SlotSize.X > KINDA_SMALL_NUMBER)
		{
			GridSize.X = SlotSize.X;
		}
	}

	InfoSlot->SetAutoSize(false);
	const FVector2D CurrentInfoSize = InfoSlot->GetSize();
	const float InfoHeight = bUsesGeneratedItemInfoPanel || CurrentInfoSize.Y <= KINDA_SMALL_NUMBER
		? ItemInfoHeight
		: CurrentInfoSize.Y;
	InfoSlot->SetSize(FVector2D(GridSize.X, InfoHeight));

	if (bUsesGeneratedItemInfoPanel)
	{
		InfoSlot->SetPosition(GridPosition + FVector2D(0.0f, Rows * TileSize + ItemInfoTopPadding));
		InfoSlot->SetZOrder(1);
	}

	if (bUsesGeneratedItemInfoPanel)
	{
		if (UCanvasPanelSlot* OwnCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this))
		{
			OwnCanvasSlot->SetSize(FVector2D(
				GridPosition.X + GridSize.X,
				GridPosition.Y + Rows * TileSize + ItemInfoTopPadding + ItemInfoHeight
			));
		}
	}
}

void UPlayerInventoryGridWidget::RefreshItemInfoPanel()
{
	EnsureItemInfoPanel();

	if (!ItemInfoBorder || !ItemInfoTitleText || !ItemInfoDescriptionText)
	{
		return;
	}

	FText Title;
	FText Description;
	if (ResolveItemInfoText(Title, Description))
	{
		LastValidItemInfoTitle = Title;
		LastValidItemInfoDescription = Description;
		bHasLastValidItemInfo = true;
		ApplyItemInfoText(Title, Description);
		return;
	}

	if (bHasLastValidItemInfo)
	{
		ApplyItemInfoText(LastValidItemInfoTitle, LastValidItemInfoDescription);
		return;
	}

	ApplyItemInfoText(FText::GetEmpty(), FText::GetEmpty());
}

void UPlayerInventoryGridWidget::ApplyItemInfoText(const FText& Title, const FText& Description)
{
	if (!ItemInfoBorder || !ItemInfoTitleText || !ItemInfoDescriptionText)
	{
		return;
	}

	ItemInfoTitleText->SetText(Title);
	ItemInfoDescriptionText->SetText(Description);
	ItemInfoBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
}

bool UPlayerInventoryGridWidget::ResolveItemInfoText(FText& OutTitle, FText& OutDescription) const
{
	if (IsValid(InventoryComponent) && InventoryComponent->IsCombineModeActive())
	{
		if (AHuntedInventoryItemBase* PendingCombineItem = InventoryComponent->GetPendingCombineItem())
		{
			if (IsValid(HoveredInfoItem))
			{
				FHuntedPlayerItemData ResultItemData;
				if (InventoryComponent->TryGetCombinationResultItemData(PendingCombineItem, HoveredInfoItem, ResultItemData))
				{
					return BuildItemInfoText(ResultItemData, OutTitle, OutDescription);
				}
			}

			return BuildItemInfoText(PendingCombineItem->GetItemData(), OutTitle, OutDescription);
		}
	}

	if (IsValid(DraggedInfoItem))
	{
		return BuildItemInfoText(DraggedInfoItem->GetItemData(), OutTitle, OutDescription);
	}

	if (IsValid(HoveredInfoItem))
	{
		return BuildItemInfoText(HoveredInfoItem->GetItemData(), OutTitle, OutDescription);
	}

	return false;
}

bool UPlayerInventoryGridWidget::BuildItemInfoText(const FHuntedPlayerItemData& ItemData, FText& OutTitle,
	FText& OutDescription) const
{
	FString TitleString = ItemData.ItemName;
	TitleString.TrimStartAndEndInline();

	FString DescriptionString = ItemData.ItemDescription;
	DescriptionString.TrimStartAndEndInline();

	if (TitleString.IsEmpty() && DescriptionString.IsEmpty())
	{
		return false;
	}

	OutTitle = FText::FromString(TitleString);
	OutDescription = FText::FromString(DescriptionString);
	return true;
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
	if (!IsValid(DraggedItem))
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
	if (!IsValid(InventoryComponent) || !GridCanvasPanel || !IsValid(PlayerReference))
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
		RefreshItemInfoPanel();
		return;
	}

	for (AHuntedInventoryItemBase* AddedItem : ItemsInInventory)
	{
		if (!IsValid(AddedItem))
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

	RefreshItemInfoPanel();
}

void UPlayerInventoryGridWidget::ResetItemInfoPanel()
{
	HoveredInfoItem = nullptr;
	DraggedInfoItem = nullptr;
	LastValidItemInfoTitle = FText::GetEmpty();
	LastValidItemInfoDescription = FText::GetEmpty();
	bHasLastValidItemInfo = false;

	EnsureItemInfoPanel();
	UpdateItemInfoPanelLayout();
	ApplyItemInfoText(FText::GetEmpty(), FText::GetEmpty());
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

void UPlayerInventoryGridWidget::SetHoveredInventoryItem(AHuntedInventoryItemBase* InItem)
{
	HoveredInfoItem = IsValid(InItem) ? InItem : nullptr;
	RefreshItemInfoPanel();
}

void UPlayerInventoryGridWidget::ClearHoveredInventoryItem(AHuntedInventoryItemBase* InItem)
{
	if (HoveredInfoItem != InItem)
	{
		return;
	}

	HoveredInfoItem = nullptr;
	RefreshItemInfoPanel();
}

void UPlayerInventoryGridWidget::SetDraggedInventoryItem(AHuntedInventoryItemBase* InItem)
{
	DraggedInfoItem = IsValid(InItem) ? InItem : nullptr;
	RefreshItemInfoPanel();
}

void UPlayerInventoryGridWidget::ClearDraggedInventoryItem(AHuntedInventoryItemBase* InItem)
{
	if (DraggedInfoItem != InItem)
	{
		return;
	}

	DraggedInfoItem = nullptr;
	RefreshItemInfoPanel();
}
