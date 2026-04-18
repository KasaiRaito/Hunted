// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "Widget/UI/HuntedWidgetBase.h"
#include "PlayerInventoryItemWidget.generated.h"

class UImage;
class UBorder;
class USizeBox;
class UCanvasPanel;
class UTextBlock;
class AHuntedPlayerCharacter;
class AHuntedInventoryItemBase;
class UDragDropOperation;

UENUM()
enum class EInventoryDragVisualState : uint8
{
	Idle,
	ValidPlacement,
	InvalidPlacement,
	DropItem
};
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

	UPROPERTY(VisibleAnywhere, meta = (BindWidgetOptional), Category= "UI")
	UTextBlock* StackCounterText;
	
	UPROPERTY(VisibleDefaultsOnly, Category= "UI")
	AHuntedPlayerCharacter* CharacterReference;
	
	AHuntedInventoryItemBase* Item;
	
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	FVector2D Size;	

	UPROPERTY(EditDefaultsOnly, Category = "UI|Drag Feedback")
	FLinearColor IdleBackgroundColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.5f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Drag Feedback")
	FLinearColor HoverBackgroundColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.2f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Drag Feedback")
	FLinearColor ValidPlacementBackgroundColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.85f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Drag Feedback")
	FLinearColor InvalidPlacementBackgroundColor = FLinearColor(0.45f, 0.45f, 0.45f, 0.85f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Drag Feedback")
	FLinearColor DropItemBackgroundColor = FLinearColor(1.0f, 0.15f, 0.15f, 0.9f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Drag Feedback")
	FLinearColor IdleImageColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Drag Feedback")
	FLinearColor HoverImageColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Drag Feedback")
	FLinearColor DragImageColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Stack Counter")
	FSlateColor StackCounterColor = FSlateColor(FLinearColor::White);
	
	bool bIsDragging = false;
	bool bHasDragStartTile = false;
	FIntPoint DragStartTopLeftTile = FIntPoint::ZeroValue;
	
protected:
	virtual void NativeConstruct() override;
	
	void AddItemWidget(AHuntedInventoryItemBase* ItemToAdd);
	
	//Mouse reaction
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void HandleDragOperationFinished(UDragDropOperation* Operation);

	void ApplyBackgroundAndImageColors(const FLinearColor& InBackgroundColor, const FLinearColor& InImageColor);
	void EnsureStackCounterWidget();
	void UpdateStackCounterVisual();

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventoryItem(AHuntedInventoryItemBase* ItemToAdd);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetDragVisualState(EInventoryDragVisualState NewState);
};
