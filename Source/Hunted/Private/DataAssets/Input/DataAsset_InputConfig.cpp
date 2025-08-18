// KasaiRaito All Rights Reserved


#include "DataAssets/Input/DataAsset_InputConfig.h"

#include "WarriorDebugHelper.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FPlayerInputActionConfig& InputActionConfig : NativeInputActions)
	{
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			Debug::Print( InInputTag.ToString() + "was called");
			return InputActionConfig.InputAction;
		}
	}

	return nullptr;
}
