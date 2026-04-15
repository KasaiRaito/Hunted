// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widget/UI/HuntedWidgetBase.h"
#include "PlayerInventoryItemWidget.generated.h"

class UImage;
class UBorder;
class USizeBox;
class UCanvasPanel;
class AHuntedPlayerCharacter;
class AHuntedInventoryItemBase;
/**
 * 
 */
UCLASS()
class HUNTED_API UPlayerInventoryItemWidget : public UHuntedWidgetBase
{
	GENERATED_BODY()
	
protected:	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	USizeBox* BackgroundSizeBox;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UBorder* BackgroundBorder;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UImage* ItemImage;
	
	UPROPERTY(VisibleDefaultsOnly, Category= "UI")
	AHuntedPlayerCharacter* CharacterReference;
	
	AHuntedInventoryItemBase* Item;
	
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	FVector2D Size;
	
protected:
	virtual void NativeConstruct() override;
	
	void AddItemWidget(AHuntedInventoryItemBase* ItemToAdd);
	
	//Mouse reaction
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
