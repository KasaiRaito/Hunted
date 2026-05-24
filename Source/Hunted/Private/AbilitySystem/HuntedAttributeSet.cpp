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
	AActor* AvatarActor = Data.Target.GetAvatarActor();
	if (!IsValid(AvatarActor))
	{
		return;
	}

	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(AvatarActor);
		//CachedPawnUIInterface = Cast<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	
	if (!CachedPawnUIInterface.IsValid())
	{
		// Gameplay effects can hit non-UI actors; skip UI updates instead of asserting in combat.
		return;
	}
	
	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	
	if (!IsValid(PawnUIComponent))
	{
		return;
	}
		
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float SafeMaxHealth = FMath::Max(GetMaxHealth(), KINDA_SMALL_NUMBER);
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, SafeMaxHealth);
		
		SetCurrentHealth(NewCurrentHealth);
		
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/SafeMaxHealth);
	}
	
	if (Data.EvaluatedData.Attribute == GetCurrentSanityAttribute())
	{
		const float SafeMaxSanity = FMath::Max(GetMaxSanity(), KINDA_SMALL_NUMBER);
		const float NewCurrentSanity = FMath::Clamp(GetCurrentSanity(), 0.f, SafeMaxSanity);
		
		SetCurrentSanity(NewCurrentSanity);
		
		if (UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentSanityChange.Broadcast(GetCurrentSanity()/SafeMaxSanity);
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		
		const float SafeMaxHealth = FMath::Max(GetMaxHealth(), KINDA_SMALL_NUMBER);
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.0f, SafeMaxHealth);
		
		SetCurrentHealth(NewCurrentHealth);
		
		/**
		const FString DebugString = FString::Printf(TEXT("Old Helath: %f, Damage Done: %f, New Current Health: %f"), 
			OldHealth, DamageDone, NewCurrentHealth);
		
		Debug::Print(DebugString, FColor::Green);
		**/
		
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/SafeMaxHealth);
		
		if (GetCurrentHealth() == 0.0f)
		{
			UHuntedFunctionLibrary::AddGameplayTagToActorIfNone(AvatarActor, HuntedGameplayTags::Shared_Status_Death);
		}
	}
}
