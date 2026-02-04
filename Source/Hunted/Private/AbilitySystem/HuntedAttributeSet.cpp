// KasaiRaito All Rights Reserved


#include "AbilitySystem/HuntedAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "HuntedFunctionLibrary.h"
#include "HuntedFunctionLibrary.h"

#include "HuntedDebugHelper.h"
#include "HuntedGameplayTags.h"

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
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		
		SetCurrentHealth(NewCurrentHealth);
	}
	
	if (Data.EvaluatedData.Attribute == GetCurrentSanityAttribute())
	{
		const float NewCurrentSanity = FMath::Clamp(GetCurrentSanity(), 0.f, GetMaxSanity());
		
		SetCurrentSanity(NewCurrentSanity);
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
		
		if (NewCurrentHealth == 0.0f)
		{
			UHuntedFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), HuntedGameplayTags::Shared_Status_Death);
		}
	}
}
