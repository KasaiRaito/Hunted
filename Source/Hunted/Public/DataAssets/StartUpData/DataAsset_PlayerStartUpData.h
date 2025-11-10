// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "GameplayTagContainer.h"
#include "DataAsset_PlayerStartUpData.generated.h"

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
/**
 * 
 */
UCLASS()
class HUNTED_API UDataAsset_PlayerStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GivenToAbilitySystemComponent(UHuntedAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override; 

private:
	UPROPERTY(EditDefaultsOnly, Category= "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FHuntedPlayerAbilitySet> PlayerStartUpAbilitySets;
};
