// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include  "AbilitySystemInterface.h"
#include "HuntedBaseCharacter.generated.h"

class UHuntedAbilitySystemComponent;
class UHuntedAttributeSet;

UCLASS()
class HUNTED_API AHuntedBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHuntedBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
	UPROPERTY(VisibleAnywhere, Blueprintable, Category = "AbilitySystem")
	UHuntedAbilitySystemComponent* HuntedAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Blueprintable, Category = "AbilitySystem")
	UHuntedAttributeSet* HuntedAttributeSet;

public:
	FORCEINLINE UHuntedAbilitySystemComponent* GetHuntedAbilitySystemComponent() const { return HuntedAbilitySystemComponent; }

	FORCEINLINE UHuntedAttributeSet* GetHuntedAttributeSet() const { return HuntedAttributeSet; }
};
