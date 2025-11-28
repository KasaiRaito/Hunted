// KasaiRaito All Rights Reserved


#include "HuntedFunctionLibrary.h"
#include "Interfaces/PawnCombatInterface.h"

UPawnCombatComponent* UHuntedFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);
	
	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	
	return nullptr;
}

UPawnCombatComponent* UHuntedFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EHuntedValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	
	OutValidType = CombatComponent? EHuntedValidType::Valid : EHuntedValidType::Invalid; 
	
	return CombatComponent;
}