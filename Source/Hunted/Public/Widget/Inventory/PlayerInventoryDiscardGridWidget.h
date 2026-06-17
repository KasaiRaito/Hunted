// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widget/Inventory/PlayerInventoryGridWidget.h"
#include "PlayerInventoryDiscardGridWidget.generated.h"

UCLASS(BlueprintType, Blueprintable)
class HUNTED_API UPlayerInventoryDiscardGridWidget : public UPlayerInventoryGridWidget
{
	GENERATED_BODY()

public:
	UPlayerInventoryDiscardGridWidget(const FObjectInitializer& ObjectInitializer);
};
