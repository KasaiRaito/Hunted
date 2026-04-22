// KasaiRaito All Rights Reserved

#pragma once

#include  "GameplayTagContainer.h"
#include "HuntedGameplayTags.h"
#include "ScalableFloat.h"

#include "HuntedStructTypes.generated.h"

class UHuntedPlayerLinkedAnimLayer;
class UHuntedGameplayAbility;
class UInputMappingContext;
class AHuntedInventoryItemBase;
class UTexture2D;
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

USTRUCT()
struct FMousePositionInTile
{
	GENERATED_USTRUCT_BODY();
	
	FMousePositionInTile()
	{
		
	};
	
	bool Vertical;
	bool Horizontal;
};

USTRUCT(BlueprintType)
struct  FHuntedPlayerItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool ItemUsable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsStackable = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsDroppable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", UIMin = "1"))
	int32 MaxStackPerCell = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat ItemMinAmount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat ItemMaxAmount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ItemAmount;
	
	void SetItemAmount();
};

UENUM(BlueprintType)
enum class EInventoryContextAction : uint8
{
	Inspect,
	Combine,
	Discard
};

USTRUCT(BlueprintType)
struct FHuntedInventoryCombinationRecipe
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AHuntedInventoryItemBase> Item1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (ClampMin = "1", UIMin = "1"))
	int32 Amount1 = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AHuntedInventoryItemBase> Item2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (ClampMin = "1", UIMin = "1"))
	int32 Amount2 = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AHuntedInventoryItemBase> Result;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (ClampMin = "1", UIMin = "1"))
	int32 ResultAmount = 1;
};

USTRUCT(BlueprintType)
struct FHuntedInventoryContextActionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EInventoryContextAction Action = EInventoryContextAction::Inspect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* Icon = nullptr;
};
