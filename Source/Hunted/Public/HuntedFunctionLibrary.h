// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HuntedTypes/HuntedEnumTypes.h"
#include "HuntedFunctionLibrary.generated.h"

//class UPawnCombatComponent;
class UHuntedAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UHuntedAbilitySystemComponent* NativeGetHuntedASCFromActor(AActor* InActor);
	
	UFUNCTION(Blueprintable, Category="Hunted|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);
	
	UFUNCTION(Blueprintable, Category="Hunted|FunctionLibrary")
	static void RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);
	
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);
	 
	UFUNCTION(BlueprintCallable, Category="Hunted|FunctionLibrary",
		meta = (DisplayName = "Does Actor Have Tag",
			ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag (AActor* InActor, FGameplayTag TagToCheck, EHuntedConfirmType& OutConfirmType);
	
	UFUNCTION(BlueprintCallable,Category = "Hunted|FunctionLibrary", 
		meta = (DisplayName = "GetPawnCombatComponentFromActor", 
			ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EHuntedValidType& OutValidType);
	
	UFUNCTION(BlueprintCallable,Category = "Hunted|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject, EHuntedInputMode InInputMode);
};
