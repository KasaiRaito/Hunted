// KasaiRaito All Rights Reserved

#include "Widget/Inventory/PlayerInventoryDiscardGridWidget.h"

UPlayerInventoryDiscardGridWidget::UPlayerInventoryDiscardGridWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GridType = EPlayerInventoryGridType::Discard;
}
