// KasaiRaito All Rights Reserved


#include "HuntedFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "AI/NavigationSystemBase.h"
#include "Components/WidgetComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"

UHuntedAbilitySystemComponent* UHuntedFunctionLibrary::NativeGetHuntedASCFromActor(AActor* InActor)
{
	if (!IsValid(InActor))
	{
		return nullptr;
	}
	
	// Blueprint callers can pass actors during teardown; return nullptr instead of crashing on CastChecked.
	return Cast<UHuntedAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UHuntedFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UHuntedAbilitySystemComponent* ASC = NativeGetHuntedASCFromActor(InActor);
	
	if (ASC && TagToAdd.IsValid() && !ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UHuntedFunctionLibrary::RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UHuntedAbilitySystemComponent* ASC = NativeGetHuntedASCFromActor(InActor);
	
	if (ASC && TagToRemove.IsValid() && ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UHuntedFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UHuntedAbilitySystemComponent* ASC = NativeGetHuntedASCFromActor(InActor);
	
	return ASC && TagToCheck.IsValid() && ASC->HasMatchingGameplayTag(TagToCheck);
}

UPawnCombatComponent* UHuntedFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	if (!IsValid(InActor))
	{
		return nullptr;
	}
	
	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	
	return nullptr;
}

void UHuntedFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck,
	EHuntedConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EHuntedConfirmType::Yes : EHuntedConfirmType::No;
}

UPawnCombatComponent* UHuntedFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
                                                                                 EHuntedValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	
	OutValidType = CombatComponent? EHuntedValidType::Valid : EHuntedValidType::Invalid; 
	
	return CombatComponent;
}

bool UHuntedFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	if (!IsValid(QueryPawn) || !IsValid(TargetPawn))
	{
		return false;
	}
	
	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());
	
	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	
	return false;
}

void UHuntedFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EHuntedInputMode InInputMode)
{
	APlayerController* PlayerController = nullptr;
	
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject , EGetWorldErrorMode::LogAndReturnNull))
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}
	
	if (!PlayerController)
	{
		return;
	}
	
	FInputModeGameOnly GameOnlyMode;
	FInputModeUIOnly UIOnlyMode;
	
	switch (InInputMode)
	{
	case EHuntedInputMode::GameOnly:
		PlayerController->SetInputMode(GameOnlyMode);
		PlayerController->bShowMouseCursor = false;
		break;
		
	case EHuntedInputMode::UIOnly:
		PlayerController->SetInputMode(UIOnlyMode);
		PlayerController->bShowMouseCursor = true;
		break;
		
	default:
		break;
	}
}
