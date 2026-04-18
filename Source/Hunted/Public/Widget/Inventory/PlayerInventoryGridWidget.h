// KasaiRaito All Rights Reserved

#pragma once

#include "Widget/UI/HuntedWidgetBase.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "HuntedTypes/HuntedStructTypes.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HuntedTypes/HuntedStructTypes.h"
#include "PlayerInventoryGridWidget.generated.h"

class UBorder;
class UCanvasPanel;
class AHuntedPlayerCharacter;
class UPlayerInventoryComponent;
class AHuntedInventoryItemBase;
class UDragDropOperation;
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

	void UpdateDraggedTargetTiles(AHuntedInventoryItemBase* DraggedItem, bool bIsValidPlacement);
	void SetDraggedTargetTiles(const TArray<FIntPoint>& InTiles, const FLinearColor& InColor);
	void SetDraggedItemVisualState(UDragDropOperation* InOperation, EInventoryDragVisualState NewState) const;
	
public:
	UFUNCTION(BlueprintCallable)
	void AddItemWidget();

	UFUNCTION(BlueprintCallable)
	void RefreshItemWidgets();

	UFUNCTION(BlueprintCallable)
	void SetDraggedSourceTiles(const TArray<FIntPoint>& InTiles);

	UFUNCTION(BlueprintCallable)
	void ClearDraggedSourceTiles();

	UFUNCTION(BlueprintCallable)
	void ClearDraggedTargetTiles();
};
