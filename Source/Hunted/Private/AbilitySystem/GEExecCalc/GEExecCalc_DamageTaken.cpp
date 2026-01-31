// KasaiRaito All Rights Reserved

#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/HuntedAttributeSet.h"
#include "HuntedGameplayTags.h"

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
	//int32 UsedKnifeComboCount = 0;
	//int32 UsedGunComboCount = 0;
	
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(HuntedGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}
		
		/** Not in use Getters **/
		/**
		if (TagMagnitude.Key.MatchesTagExact(HuntedGameplayTags::Player_SetByCaller_AttackType_BasicKnife))
		{
			UsedKnifeComboCount = TagMagnitude.Value;
		}
		
		if (TagMagnitude.Key.MatchesTagExact(HuntedGameplayTags::Player_SetByCaller_AttackType_BasicGun))
		{
			UsedGunComboCount = TagMagnitude.Value;
		}
		**/
	}
	
	float TargetDefencePower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetHuntedDamageCapture().DefencePowerDef,
		EvaluateParameters,
		TargetDefencePower
	);
	
	
}
