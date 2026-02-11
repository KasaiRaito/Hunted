// KasaiRaito All Rights Reserved


#include "Widget/PlayerInventoryGrid.h"

#include "Characters/HuntedPlayerCharacter.h"
#include "HuntedDebugHelper.h"
#include "IPropertyTable.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UPlayerInventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetUpInventoryGrid();
	CreateLineSegments();
}

void UPlayerInventoryGrid::SetUpInventoryGrid()
{
	AHuntedPlayerCharacter* PlayerReference;
	
	PlayerReference = Cast<AHuntedPlayerCharacter>(GetOwningPlayerPawn());
	
	if (!PlayerReference)
	{
		Debug::Print("[PlayerInventoryGrid] Can't construct player inventory component");
		return;
	}

	UPlayerInventoryComponent* InventoryComponent = PlayerReference->PlayerInventoryComponent;
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
	
	LineStructData = new FLines();
	StartX = {};
	StartY = {};
	EndX= {};
	EndY= {};
	
	UCanvasPanelSlot* BorderAsCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
	if (BorderAsCanvasSlot)
	{
		BorderAsCanvasSlot->SetSize(FVector2D(NewWidth, NewHeight));
	}
}

void UPlayerInventoryGrid::CreateLineSegments()
{
	for (int8 Column = 0; Column <= Columns; Column++)
	{
		float X = Column * TileSize;
		
		LineStructData->XLines.Add(FVector2D(X, X));
		LineStructData->YLines.Add(FVector2D(0.0f, Rows * TileSize));
	}
	
	for (int8 Row = 0; Row <= Rows; Row++)
	{
		float Y = Row * TileSize;
		
		LineStructData->XLines.Add(FVector2D(0.0f, Columns * TileSize));
		LineStructData->YLines.Add(FVector2D(Y, Y));
	}
	
	for (const FVector2D Elements : LineStructData->XLines)
	{
		StartX.Add(Elements.X);
		EndX.Add(Elements.Y); // use line end X instead of start
		
	}
	for (const FVector2D Elements : LineStructData->YLines)
	{
		StartY.Add(Elements.X);
		EndY.Add(Elements.Y);
	}
	
}

int32 UPlayerInventoryGrid::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	
	FPaintContext PaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	
	if (!LineStructData)
	{
		return LayerId;
	}

	FVector2D Offset = GridBorder->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.0f,0.0f)); // align to border
	
	for (int32 i = 0; i < LineStructData->XLines.Num(); i++)
	{
		UWidgetBlueprintLibrary::DrawLine(PaintContext, 
			FVector2D(StartX[i], StartY[i]) + Offset, // matching start coords
			FVector2D(EndX[i], EndY[i]) + Offset, CustomLineColor,
			false, InventoryLineThickness); // matching end coords
	}
		
	return int32();
}
