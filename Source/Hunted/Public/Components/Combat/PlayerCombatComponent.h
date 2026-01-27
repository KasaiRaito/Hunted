// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class AHuntedPlayerWeaponBase;
/**
 * 
 */
UCLASS()
class HUNTED_API UPlayerCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	//MIGHT HAVE TO CHANGE LOGIC TO A CHILD CLASS SPECIFIC WEAPON
	UFUNCTION(BlueprintCallable, Category = "Hunted|Combat")
	AHuntedPlayerWeaponBase* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const;
	
	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;
};
