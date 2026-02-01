// KasaiRaito All Rights Reserved

#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"

#include <string>

#include "AbilitySystem/HuntedAttributeSet.h"
#include "HuntedGameplayTags.h"

#include "HuntedDebugHelper.h"

struct FHuntedDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefencePower)
	
	FHuntedDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UHuntedAttributeSet, 
			AttackPower, 
			Source, 
			false
		);
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UHuntedAttributeSet, 
			DefencePower, 
			Target, 
			false
		);
	}
};

static const FHuntedDamageCapture& GetHuntedDamageCapture()
{
	static FHuntedDamageCapture HuntedDamageCapture;
	return HuntedDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	/** Fast Way of Doing Capture **/
	RelevantAttributesToCapture.Add(GetHuntedDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetHuntedDamageCapture().DefencePowerDef);
	
	/** Slow Way of Doing Capture **/
	/**FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
		UHuntedAttributeSet::StaticClass(), 
		GET_MEMBER_NAME_CHECKED(UHuntedAttributeSet,AttackPower)	
	);
	
	FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
		AttackPowerProperty,
		EGameplayEffectAttributeCaptureSource::Source,
		false
	);
	
	RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);
	**/
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	
	/**
	EffectSpec.GetContext().GetSourceObject();
	EffectSpec.GetContext().GetAbility();
	EffectSpec.GetContext().GetInstigator();
	EffectSpec.GetContext().GetEffectCauser()
	**/
	
	FAggregatorEvaluateParameters EvaluateParameters;
	
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	float SourceAttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetHuntedDamageCapture().AttackPowerDef,
		EvaluateParameters,
		SourceAttackPower
	);

	float  BaseDamage = 0.0f;
	float DamageScalar = 0.0f;
	FGameplayTagContainer HitTypeTags;
	HitTypeTags.AddTag(HuntedGameplayTags::Player_SetByCaller_AttackType_Head);
	HitTypeTags.AddTag(HuntedGameplayTags::Player_SetByCaller_AttackType_Body);
	HitTypeTags.AddTag(HuntedGameplayTags::Player_SetByCaller_AttackType_Leg);
	
	
	//int32 UsedKnifeComboCount = 0;
	//int32 UsedGunComboCount = 0;
	
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(HuntedGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}
			
		if (TagMagnitude.Key.MatchesAny(HitTypeTags))
		{
			DamageScalar = TagMagnitude.Value;
	
			FString DamageAsString = FString::SanitizeFloat(DamageScalar);
			Debug::Print(TEXT( "Damage Scalar = " + DamageAsString), FColor::Purple);
		}
	}
	
	float TargetDefencePower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetHuntedDamageCapture().DefencePowerDef,
		EvaluateParameters,
		TargetDefencePower
	);
	
	if (BaseDamage == 0.0f)
	{
		Debug::Print(TEXT("Base Damage IS ZERO"));
	}
	
	const float FinalDamageDone = (BaseDamage* DamageScalar);
	FString FloatAsString = FString::SanitizeFloat(FinalDamageDone);
	
	Debug::Print(TEXT( "FinalDamage= " + FloatAsString));
}