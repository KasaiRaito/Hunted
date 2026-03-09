// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include  "AbilitySystemInterface.h"

#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"

#include "DrawDebugHelpers.h"
#include "HuntedGameplayTags.h"

#include "HuntedBaseCharacter.generated.h"


class UHuntedAbilitySystemComponent;
class UHuntedAttributeSet;
class UDataAsset_StartUpDataBase;

UCLASS()
class HUNTED_API AHuntedBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface,public IPawnUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHuntedBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface
	
	//~ Begin IPawnCombatInterface Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface
	
	//~ Begin IPawnUIInterface Interface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UHuntedAbilitySystemComponent* HuntedAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Blueprintable, Category = "AbilitySystem")
	UHuntedAttributeSet* HuntedAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;
	
	UPROPERTY()
	bool AimingState = false;

public:
	FORCEINLINE UHuntedAbilitySystemComponent* GetHuntedAbilitySystemComponent() const { return HuntedAbilitySystemComponent; }
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool GetAimingState() const { return AimingState; };

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetAimingState(bool bAimingState) { AimingState = bAimingState; };
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AActor* HuntedRayCast(FVector startLocation = FVector::ZeroVector, FVector rotation = FVector::ZeroVector, float range = 1000.f);
	
	/*
	UFUNCTION(Blueprintable, Category = "Weapon")
	HuntedGameplayTags GetDamageTagToApply(USkeletalMeshSocket PartHitten);
	*/
	
	FORCEINLINE UHuntedAttributeSet* GetHuntedAttributeSet() const { return HuntedAttributeSet; }
};
