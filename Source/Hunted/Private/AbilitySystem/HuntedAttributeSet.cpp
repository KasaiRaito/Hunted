// KasaiRaito All Rights Reserved


#include "AbilitySystem/HuntedAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "HuntedFunctionLibrary.h"
#include "Interfaces/PawnUIInterface.h"

#include "HuntedGameplayTags.h"

#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/PlayerUIComponent.h"

#include "HuntedDebugHelper.h"

UHuntedAttributeSet::UHuntedAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentSanity(1.f);
	InitMaxSanity(1.f);
	InitAttackPower(1.f);
	InitDefencePower(1.f);
}

void UHuntedAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
		//CachedPawnUIInterface = Cast<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't impement IPawnUIInterface"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
	
	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	
	checkf(PawnUIComponent, TEXT("Could not Extract a PawnUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
		
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		
		SetCurrentHealth(NewCurrentHealth);
		
		if (UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentSanityChange.Broadcast(GetCurrentSanity()/GetMaxSanity());
		}
		
	}
	
	if (Data.EvaluatedData.Attribute == GetCurrentSanityAttribute())
	{
		const float NewCurrentSanity = FMath::Clamp(GetCurrentSanity(), 0.f, GetMaxSanity());
		
		SetCurrentSanity(NewCurrentSanity);
		
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentSanity()/GetMaxSanity());
	}
	
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.0f, GetMaxHealth());
		
		SetCurrentHealth(NewCurrentHealth);
		
		const FString DebugString = FString::Printf(TEXT("Old Helath: %f, Damage Done: %f, New Current Health: %f"), 
			OldHealth, DamageDone, NewCurrentHealth);
		
		Debug::Print(DebugString, FColor::Green);
		
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(NewCurrentHealth/GetMaxHealth());
		
		if (NewCurrentHealth == 0.0f)
		{
			UHuntedFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), HuntedGameplayTags::Shared_Status_Death);
		}
	}
}
