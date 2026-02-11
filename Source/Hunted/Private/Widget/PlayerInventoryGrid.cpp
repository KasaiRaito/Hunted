// KasaiRaito All Rights Reserved


#include "Widget/PlayerInventoryGrid.h"

#include "Characters/HuntedPlayerCharacter.h"
#include "HuntedDebugHelper.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UPlayerInventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();
	
	AHuntedPlayerCharacter* PlayerReference;
	
	PlayerReference = Cast<AHuntedPlayerCharacter>(GetOwningPlayerPawn());
	
	UPlayerInventoryComponent* InventoryComponent;
	InventoryComponent = PlayerReference->PlayerInventoryComponent;
	
	if (!InventoryComponent || !PlayerReference)
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
	Start = {};
	End= {};
	
	UCanvasPanelSlot* BorderAsCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
	BorderAsCanvasSlot->SetSize(FVector2D(NewWidth, NewHeight));
}

void UPlayerInventoryGrid::CreateLineSegments()
{
}

int32 UPlayerInventoryGrid::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);
}
