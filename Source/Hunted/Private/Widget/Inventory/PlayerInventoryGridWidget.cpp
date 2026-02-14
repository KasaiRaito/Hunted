// KasaiRaito All Rights Reserved


#include "Widget/Inventory/PlayerInventoryGridWidget.h"

#include "Characters/HuntedPlayerCharacter.h"
#include "HuntedDebugHelper.h"
#include "IPropertyTable.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UPlayerInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetUpInventoryGrid();
	if (bGridDirty)
	{
		CreateLineSegments();
	}
}

void UPlayerInventoryGridWidget::SetUpInventoryGrid()
{
	AHuntedPlayerCharacter* PlayerReference;
	
	PlayerReference = Cast<AHuntedPlayerCharacter>(GetOwningPlayerPawn());
	
	if (!PlayerReference)
	{
		Debug::Print("[PlayerInventoryGrid] Can't construct player inventory component");
		return;
	}

	UPlayerInventoryComponent* InventoryComponent = PlayerReference->GetPlayerInventoryComponent();
	if (!InventoryComponent)
	{
		Debug::Print("[PlayerInventoryGrid] Can't construct player inventory component");
		return;
	}
	
	Columns = InventoryComponent->Columns;
	Rows = InventoryComponent->Rows;
	TileSize = InventoryComponent->TileSize;
	
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
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	
	FPaintContext PaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	
	if (!GridBorder)
	{
		return LayerId;
	}

	int32 DrawCount = FMath::Min(StartX.Num(), StartY.Num());
	DrawCount = FMath::Min(DrawCount, EndX.Num());
	DrawCount = FMath::Min(DrawCount, EndY.Num());
	if (DrawCount == 0)
	{
		return LayerId;
	}

	FVector2D Offset = GridBorder->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.0f,0.0f));
	
	for (int32 i = 0; i < DrawCount; i++)
	{
		UWidgetBlueprintLibrary::DrawLine(PaintContext, 
			FVector2D(StartX[i], StartY[i]) + Offset,
			FVector2D(EndX[i], EndY[i]) + Offset, CustomLineColor,
			false, InventoryLineThickness);
	}
		
	return int32();
}
