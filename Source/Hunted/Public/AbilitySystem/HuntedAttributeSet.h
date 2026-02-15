// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "HuntedAttributeSet.generated.h"

class IPawnUIInterface;


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UHuntedAttributeSet();
	
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UHuntedAttributeSet, CurrentHealth)
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHuntedAttributeSet, MaxHealth)
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentSanity;
	ATTRIBUTE_ACCESSORS(UHuntedAttributeSet, CurrentSanity)
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxSanity;
	ATTRIBUTE_ACCESSORS(UHuntedAttributeSet, MaxSanity)
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UHuntedAttributeSet, AttackPower)
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData DefencePower;
	ATTRIBUTE_ACCESSORS(UHuntedAttributeSet, DefencePower)
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UHuntedAttributeSet, DamageTaken)
	
private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
};
