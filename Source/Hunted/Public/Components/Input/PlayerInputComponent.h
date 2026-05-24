// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"

#include "HuntedDebugHelper.h"
#include "PlayerInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class HUNTED_API UPlayerInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag,
		ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject,
	CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
};

template <class UserObject, typename CallBackFunc>
void UPlayerInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,
	const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallBackFunc Func)
{
	if (!InInputConfig)
	{
		// Blueprint/player setup can be incomplete in PIE; skip binding instead of crashing.
		return;
	}

	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

template <class UserObject, typename CallbackFunc>
void UPlayerInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig,
	UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	if (!InInputConfig)
	{
		return;
	}

	for (const FPlayerInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsValid())
		{
			Debug::Print("Error");
			continue;
		}

		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject,
			InputPressedFunc, AbilityInputActionConfig.InputTag);

		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject,
			InputReleasedFunc, AbilityInputActionConfig.InputTag);
	}
}
