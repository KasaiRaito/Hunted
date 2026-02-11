// KasaiRaito All Rights Reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "HuntedTypes/HuntedStructTypes.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HuntedTypes/HuntedStructTypes.h"
#include "PlayerInventoryGrid.generated.h"

class UCanvasPanel;
class UBorder;

/**
 * 
 */
UCLASS()
class HUNTED_API UPlayerInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UBorder* GridBorder;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category= "UI")
	UCanvasPanel* GridCanvasPanel;

	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	int8 Columns;
	
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	int8 Rows;
	
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	float TileSize;
		
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	TArray<float> StartX;
	
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	TArray<float> StartY;
	
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	TArray<float> EndX;
	
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	TArray<float> EndY;
	
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	FLinearColor CustomLineColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);
	
	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget), Category= "UI")
	float InventoryLineThickness = 5.0f;
	
	FLines* LineStructData;
	
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void SetUpInventoryGrid();
	
	UFUNCTION()
	void CreateLineSegments();
		
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override; 
	
};
