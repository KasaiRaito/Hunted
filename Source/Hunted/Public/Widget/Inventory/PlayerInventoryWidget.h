// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widget/UI/HuntedWidgetBase.h"
#include "PlayerInventoryWidget.generated.h"

class UBorder;
class UCanvasPanel;
class UBackgroundBlur;
class AHuntedPlayerCharacter;
class AHuntedInventoryItemBase;

UCLASS()
class HUNTED_API UPlayerInventoryWidget : public UHuntedWidgetBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UBorder* BackgroundBorder;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UBackgroundBlur* BackgroundBlur;
	
	AHuntedInventoryItemBase* SpawnedItem;
	
protected:
	AHuntedPlayerCharacter* CharacterReference;
	
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	FHitResult GetLocationBelow(FVector Start) const;
};
