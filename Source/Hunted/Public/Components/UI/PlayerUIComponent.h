// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "PlayerUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBulletMagCountChangedDelegate, int, ShotCount, bool, IsMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBulletBagCountChangedDelegate, int, ShotCount, bool, IsMax);

/**
 * 
 */
UCLASS()
class HUNTED_API UPlayerUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentSanityChange;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnBulletMagCountChangedDelegate OnBulletMagCountChanged;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnBulletBagCountChangedDelegate OnBulletBagCountChanged;
};
