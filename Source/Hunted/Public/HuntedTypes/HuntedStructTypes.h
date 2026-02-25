// KasaiRaito All Rights Reserved

#pragma once

#include  "GameplayTagContainer.h"
#include "ScalableFloat.h"

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};

/** Inventory Structs **/
USTRUCT()
struct FLines
{
	GENERATED_USTRUCT_BODY();
	
	FLines()
	{
		
	};
	
	TArray<FVector2D> XLines;
	TArray<FVector2D> YLines;
	
};

USTRUCT(BlueprintType)
struct  FHuntedPlayerItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool ItemInventory;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool ItemUsable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsStackable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* ItemMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemImage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ItemAmount;	
	
	bool IsValid() const;
};
