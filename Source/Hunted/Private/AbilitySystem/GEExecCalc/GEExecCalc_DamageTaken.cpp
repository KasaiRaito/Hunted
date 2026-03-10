// KasaiRaito All Rights Reserved

#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/HuntedAttributeSet.h"
#include "HuntedGameplayTags.h"

#include "HuntedDebugHelper.h"

struct FHuntedDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefencePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	
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
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(
			UHuntedAttributeSet, 
			DamageTaken,
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
	RelevantAttributesToCapture.Add(GetHuntedDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetHuntedDamageCapture().DefencePowerDef);
	RelevantAttributesToCapture.Add(GetHuntedDamageCapture().DamageTakenDef);
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
	HitTypeTags.AddTag(HuntedGameplayTags::Player_SetByCaller_AttackType_BasicKnife);
	HitTypeTags.AddTag(HuntedGameplayTags::Player_SetByCaller_AttackType_Head);
	HitTypeTags.AddTag(HuntedGameplayTags::Player_SetByCaller_AttackType_Body);
	HitTypeTags.AddTag(HuntedGameplayTags::Player_SetByCaller_AttackType_Leg);
	
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(HuntedGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			//Debug::Print(TEXT( "Base Damage"), BaseDamage);
		}
			
		if (TagMagnitude.Key.MatchesAny(HitTypeTags))
		{
			DamageScalar = TagMagnitude.Value;
			//Debug::Print(TEXT( "Damage Scalar"), DamageScalar);
		}
		else
		{
			Debug::Print(TEXT( "DIDNT GET DAMAGE SCALAR FROM TAG"), DamageScalar);
			DamageScalar = 1.0f;
		}
	}
	
	float TargetDefencePower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetHuntedDamageCapture().DefencePowerDef,
		EvaluateParameters,
		TargetDefencePower
	);
	//Debug::Print(TEXT( "Enemy Defence"), TargetDefencePower);
	
	const float FinalDamageDone = (BaseDamage* DamageScalar) - TargetDefencePower;
	
	//Debug::Print(TEXT( "Final Damage"), FinalDamageDone);
		
	if (FinalDamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetHuntedDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)	
		);
	}
}