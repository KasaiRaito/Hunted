// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Styling/SlateColor.h"
#include "HuntedTypes/HuntedStructTypes.h"
#include "Widget/UI/HuntedWidgetBase.h"
#include "PlayerInventoryItemWidget.generated.h"

class UImage;
class UBorder;
class USizeBox;
class UCanvasPanel;
class UTextBlock;
class UButton;
class UVerticalBox;
class UHorizontalBox;
class AHuntedPlayerCharacter;
class AHuntedInventoryItemBase;

UCLASS()
class HUNTED_API UPlayerInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	AHuntedInventoryItemBase* DraggedItem = nullptr;

	UPROPERTY(Transient)
	FIntPoint OriginalItemSize = FIntPoint::ZeroValue;

	UPROPERTY(Transient)
	FIntPoint DragStartTopLeftTile = FIntPoint::ZeroValue;

	UPROPERTY(Transient)
	bool bHasDragStartTile = false;

	UPROPERTY(Transient)
	bool bWasRightMouseButtonDown = false;

	UPROPERTY(Transient)
	bool bIsRotated = false;

	UFUNCTION()
	void InitializeInventoryDrag(AHuntedInventoryItemBase* InDraggedItem, bool bInHasDragStartTile,
		FIntPoint InDragStartTopLeftTile);

	UFUNCTION()
	bool ToggleDraggedItemRotation();

	UFUNCTION()
	void RestoreOriginalItemSize();

	void HandleRotationInput(const FPointerEvent& PointerEvent);

	virtual void Dragged_Implementation(const FPointerEvent& PointerEvent) override;
};

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

	UPROPERTY(EditDefaultsOnly, Category = "UI|Combine Feedback")
	FLinearColor CombineSelectedBackgroundColor = FLinearColor(0.45f, 0.1f, 0.65f, 0.95f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Combine Feedback")
	FLinearColor CombineSelectedImageColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Combine Feedback")
	FLinearColor CombineCompatibleBackgroundColor = FLinearColor(0.35f, 0.2f, 0.5f, 0.55f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Combine Feedback")
	FLinearColor CombineCompatibleImageColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.95f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Combine Feedback")
	FLinearColor CombineIncompatibleBackgroundColor = FLinearColor(0.18f, 0.18f, 0.18f, 0.65f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Combine Feedback")
	FLinearColor CombineIncompatibleImageColor = FLinearColor(0.45f, 0.45f, 0.45f, 0.75f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Context Menu")
	UTexture2D* InspectActionIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Context Menu")
	UTexture2D* CombineActionIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Context Menu")
	UTexture2D* DiscardActionIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Context Menu", meta = (ClampMin = "8.0", UIMin = "8.0"))
	float ContextMenuIconSize = 14.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Context Menu", meta = (ClampMin = "0.5", UIMin = "0.5"))
	float ContextMenuFontSize = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Context Menu")
	FMargin ContextMenuPadding = FMargin(4.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Context Menu")
	FMargin ContextMenuEntryPadding = FMargin(4.0f, 2.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Context Menu")
	FVector2D ContextMenuOffset = FVector2D(8.0f, 0.0f);
	
	bool bIsDragging = false;
	bool bHasDragStartTile = false;
	FIntPoint DragStartTopLeftTile = FIntPoint::ZeroValue;

	UPROPERTY(Transient)
	UBorder* ContextMenuBorder = nullptr;

	UPROPERTY(Transient)
	UVerticalBox* ContextMenuBox = nullptr;

	UPROPERTY(Transient)
	TArray<FHuntedInventoryContextActionEntry> ContextMenuActions;

	int32 CachedCanvasSlotZOrder = 0;
	bool bHasCachedCanvasSlotZOrder = false;
	int32 CachedInventoryGridSlotZOrder = 0;
	bool bHasCachedInventoryGridSlotZOrder = false;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
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
	void EnsureContextMenuWidget();
	void RebuildContextMenuEntries();
	void ToggleContextMenu();
	void HideContextMenu();
	bool IsContextMenuVisible() const;
	void RaiseContextMenuLayer();
	void RestoreContextMenuLayer();
	void RefreshCombineVisualState();
	void ApplyCombineVisualState(bool bIsSelectedSource, bool bCanCombine);
	FHuntedInventoryContextActionEntry MakeContextActionEntry(EInventoryContextAction Action, const FText& Label, UTexture2D* Icon) const;

	UFUNCTION()
	void HandleInspectClicked();

	UFUNCTION()
	void HandleCombineClicked();

	UFUNCTION()
	void HandleDiscardClicked();

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventoryItem(AHuntedInventoryItemBase* ItemToAdd);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetDragVisualState(EInventoryDragVisualState NewState);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RefreshItemVisualLayout();

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void BP_OnInspectRequested(AHuntedInventoryItemBase* InspectedItem);

	static bool IsAnyContextMenuOpen();
};
