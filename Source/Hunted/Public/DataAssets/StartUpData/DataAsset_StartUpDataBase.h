// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

/**
 * 
 */

class UHuntedGameplayAbility;
class UHuntedAbilitySystemComponent;

UCLASS()
class HUNTED_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GivenToAbilitySystemComponent(UHuntedAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
	
protected:
	UPROPERTY(EditDefaultsOnly,Category= "StartUpData")
	TArray<TSubclassOf<UHuntedGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly,Category= "StartUpData")
	TArray<TSubclassOf<UHuntedGameplayAbility>> ReactiveAbilities;

	void GranAbilities(const TArray< TSubclassOf <UHuntedGameplayAbility> >& InAbilitiesToGive, UHuntedAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
