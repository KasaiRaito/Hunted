// KasaiRaito All Rights Reserved

#pragma once

#include "Widget/UI/HuntedWidgetBase.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "HuntedTypes/HuntedStructTypes.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HuntedTypes/HuntedEnumTypes.h"
#include "HuntedTypes/HuntedStructTypes.h"
#include "PlayerInventoryGridWidget.generated.h"

class UBorder;
class UCanvasPanel;
class UImage;
class UTextBlock;
class UVerticalBox;
class AHuntedPlayerCharacter;
class UPlayerInventoryComponent;
class AHuntedInventoryItemBase;
class UDragDropOperation;
class UMaterialInterface;
enum class EInventoryDragVisualState : uint8;

/**
 * 
 */
UCLASS()
class HUNTED_API UPlayerInventoryGridWidget : public UHuntedWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY()
	UBorder* GridBorder;
	
	UPROPERTY()
	UCanvasPanel* GridCanvasPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Item Info")
	UBorder* ItemInfoBorder = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Item Info")
	UVerticalBox* ItemInfoBox = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Item Info")
	UTextBlock* ItemInfoTitleText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Item Info")
	UTextBlock* ItemInfoDescriptionText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI|Item Info")
	UImage* ItemInfoResultImage = nullptr;

	UPROPERTY()
	AHuntedPlayerCharacter* PlayerReference;
	
	UPROPERTY()
	UPlayerInventoryComponent* InventoryComponent;
	
	UPROPERTY()
	UPanelSlot* PanelSlot;
	
	UPROPERTY()
	int8 Columns;
	
	UPROPERTY()
	int8 Rows;
	
	UPROPERTY()
	float TileSize;
		
	UPROPERTY()
	TArray<float> StartX;
	
	UPROPERTY()
	TArray<float> StartY;
	
	UPROPERTY()
	TArray<float> EndX;
	
	UPROPERTY()
	TArray<float> EndY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|Grid")
	EPlayerInventoryGridType GridType = EPlayerInventoryGridType::Inventory;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category= "UI")
	FLinearColor CustomLineColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category= "UI")
	float InventoryLineThickness = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category= "UI")
	FLinearColor DraggedSourceCellColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.35f);

	UPROPERTY(EditDefaultsOnly, Category= "UI")
	FLinearColor DraggedTargetValidCellColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.30f);

	UPROPERTY(EditDefaultsOnly, Category= "UI")
	FLinearColor DraggedTargetInvalidCellColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.30f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Item Info")
	FLinearColor ItemInfoBackgroundColor = FLinearColor(0.02f, 0.02f, 0.02f, 0.82f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Item Info")
	FLinearColor ItemInfoTitleColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Item Info")
	FLinearColor ItemInfoDescriptionColor = FLinearColor(0.78f, 0.78f, 0.78f, 1.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Item Info", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ItemInfoTopPadding = 12.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Item Info", meta = (ClampMin = "16.0", UIMin = "16.0"))
	float ItemInfoHeight = 96.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Item Info")
	FMargin ItemInfoContentPadding = FMargin(12.0f, 8.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Item Info", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float ItemInfoTitleFontSize = 18.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Item Info", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float ItemInfoDescriptionFontSize = 12.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Item Info")
	FVector2D ItemInfoResultImageSize = FVector2D(64.0f, 64.0f);
	
	FLines LineStructData;

	bool bGridDirty = true;

	UPROPERTY(Transient)
	TArray<FIntPoint> DraggedSourceTiles;

	UPROPERTY(Transient)
	bool bDrawDraggedSourceTiles = false;

	UPROPERTY(Transient)
	TArray<FIntPoint> DraggedTargetTiles;

	UPROPERTY(Transient)
	bool bDrawDraggedTargetTiles = false;

	UPROPERTY(Transient)
	FLinearColor DraggedTargetTilesColor = FLinearColor::Transparent;

	UPROPERTY(Transient)
	AHuntedInventoryItemBase* HoveredInfoItem = nullptr;

	UPROPERTY(Transient)
	AHuntedInventoryItemBase* DraggedInfoItem = nullptr;

	UPROPERTY(Transient)
	FText LastValidItemInfoTitle;

	UPROPERTY(Transient)
	FText LastValidItemInfoDescription;

	UPROPERTY(Transient)
	bool bHasLastValidItemInfo = false;

	UPROPERTY(Transient)
	bool bUsesGeneratedItemInfoPanel = false;
	
	/** Cached Values **/
	int8 CachedColumns = 0;
	int8 CachedRows = 0;
	float CachedTileSize = 0.0f;
	
	FIntPoint DraggedItemTopLeftTile;
	
	//FVector2D ScreenPosition;
	//FVector2D LocalPosition;
	
	FMousePositionInTile CachedMousePositionInTile;
	
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void SetUpInventoryGrid();
	
	UFUNCTION()
	void CreateLineSegments();
		
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, 
		const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, 
		int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	bool IsRoomAvailableFroPayload(AHuntedInventoryItemBase* Item) const;
	
	FMousePositionInTile MousePositionInTileResult(FVector2D MousePosition);

	void EnsureItemInfoPanel();
	void UpdateItemInfoPanelLayout();
	void RefreshItemInfoPanel();
	void ApplyItemInfoContent(const FText& Title, const FText& Description, UMaterialInterface* ResultIcon);
	bool ResolveItemInfoContent(FText& OutTitle, FText& OutDescription, UMaterialInterface*& OutResultIcon) const;
	bool BuildItemInfoText(const FHuntedPlayerItemData& ItemData, FText& OutTitle, FText& OutDescription) const;
	void UpdateDraggedTargetTiles(AHuntedInventoryItemBase* DraggedItem, bool bIsValidPlacement);
	void SetDraggedTargetTiles(const TArray<FIntPoint>& InTiles, const FLinearColor& InColor);
	void SetDraggedItemVisualState(UDragDropOperation* InOperation, EInventoryDragVisualState NewState) const;
	
public:
	UFUNCTION(BlueprintPure, Category = "Inventory|Grid")
	EPlayerInventoryGridType GetInventoryGridType() const { return GridType; }

	UFUNCTION(BlueprintCallable)
	void AddItemWidget();

	UFUNCTION(BlueprintCallable)
	void RefreshItemWidgets();

	UFUNCTION(BlueprintCallable)
	void ResetItemInfoPanel();

	UFUNCTION(BlueprintCallable)
	void SetDraggedSourceTiles(const TArray<FIntPoint>& InTiles);

	UFUNCTION(BlueprintCallable)
	void ClearDraggedSourceTiles();

	UFUNCTION(BlueprintCallable)
	void ClearDraggedTargetTiles();

	UFUNCTION(BlueprintCallable)
	void SetHoveredInventoryItem(AHuntedInventoryItemBase* InItem);

	UFUNCTION(BlueprintCallable)
	void ClearHoveredInventoryItem(AHuntedInventoryItemBase* InItem);

	UFUNCTION(BlueprintCallable)
	void SetDraggedInventoryItem(AHuntedInventoryItemBase* InItem);

	UFUNCTION(BlueprintCallable)
	void ClearDraggedInventoryItem(AHuntedInventoryItemBase* InItem);
};
