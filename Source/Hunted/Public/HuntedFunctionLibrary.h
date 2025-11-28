// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HuntedTypes/WarriorEnumTypes.h"
#include "HuntedFunctionLibrary.generated.h"

class UPawnCombatComponent;
/**
 * 
 */
UCLASS()
class HUNTED_API UHuntedFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);
	 
	UFUNCTION(BlueprintCallable,Category = "Warrior|FunctionLibrary", 
		meta = (DisplayName = "GetPawnCombatComponentFromActor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EHuntedValidType& OutValidType);
};
