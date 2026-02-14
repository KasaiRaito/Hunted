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

class UCanvasPanel;
class UBorder;

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
	
	FLines LineStructData;

	bool bGridDirty = true;
	
	/** Cached Values **/
	int8 CachedColumns = 0;
	int8 CachedRows = 0;
	float CachedTileSize = 0.0f;
	
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void SetUpInventoryGrid();
	
	UFUNCTION()
	void CreateLineSegments();
		
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override; 
	
};
