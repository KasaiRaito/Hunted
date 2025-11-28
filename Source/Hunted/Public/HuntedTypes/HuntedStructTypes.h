// KasaiRaito All Rights Reserved

#pragma once

#include  "GameplayTagContainer.h"
#include "HuntedStructTypes.generated.h"

class UHuntedPlayerLinkedAnimLayer;
class UHuntedGameplayAbility;
class UInputMappingContext;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FHuntedPlayerAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHuntedGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

class UHuntedPlayerLinkedAnimLayer;

USTRUCT(BlueprintType)
struct FHuntedPlayerWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHuntedPlayerLinkedAnimLayer> WeaponAnimLayerToLink;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FHuntedPlayerAbilitySet> DefaultWeaponAbilities;
};