// KasaiRaito All Rights Reserved

#pragma once

UENUM()
enum class EHuntedConfirmType : uint8
{
	Yes,
	No,
};

UENUM()
enum class EHuntedValidType : uint8
{
	Valid,
	Invalid,
};

UENUM()
enum class EHuntedSuccessType : uint8
{
	Successful,
	Failed,
};

UENUM(BlueprintType)
enum class EHuntedInputMode: uint8
{
	GameOnly,
	UIOnly
};

UENUM(BlueprintType)
enum class EPlayerInventoryGridType : uint8
{
	Inventory,
	Discard
};
