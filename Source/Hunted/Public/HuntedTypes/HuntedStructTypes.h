// KasaiRaito All Rights Reserved

#pragma once
#include "HuntedStructTypes.generated.h"

class UHuntedPlayerLinkedAnimLayer;

USTRUCT(BlueprintType)
struct FHuntedPlayerWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHuntedPlayerLinkedAnimLayer> WeaponAnimLayerToLink;
};