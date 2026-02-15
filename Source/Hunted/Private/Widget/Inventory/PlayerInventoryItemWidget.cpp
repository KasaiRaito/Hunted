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

#include "HuntedDebugHelper.h"
#include "Components/CanvasPanelSlot.h"

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
	
	ItemImage->SetBrushFromMaterial(ItemToAdd->GetIcon());
	
	Size = FVector2D( ItemToAdd->GetItemInventorySize() * CharacterReference->GetPlayerInventoryComponent()->GetTileSize());
	
	BackgroundSizeBox->SetWidthOverride(Size.X);
	BackgroundSizeBox->SetHeightOverride(Size.Y);
	
	UCanvasPanelSlot* ImageAsCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemImage);
	
	ImageAsCanvasSlot->SetSize(Size);
}
