// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/HuntedWeaponBase.h"
#include "HuntedTypes/HuntedStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "HuntedPlayerWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class HUNTED_API AHuntedPlayerWeaponBase : public AHuntedWeaponBase
{
	GENERATED_BODY()

public:
	//MIGHT HAVE TO CHANGE LOGIC TO A CHILD CLASS SPECIFIC WEAPON
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FHuntedPlayerWeaponData PlayerWeaponData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	int MagSize = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponData")
	int ShootCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponData")
	float range;
	
	UFUNCTION(BlueprintCallable)
	void AssignGratedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);
	
	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandle() const;
	
	UFUNCTION(BlueprintCallable)
	int GetMagSize() const { return MagSize; };
	
	UFUNCTION(BlueprintCallable)
	void SetMagSize(int val) { MagSize = val; }; 
	
	UFUNCTION(BlueprintCallable)
	int GetShootCount() const {return ShootCount;};
	
	UFUNCTION(BlueprintCallable)
	void SetShootCount(int val) { ShootCount = val; };
	
	UFUNCTION(BlueprintCallable)
	float GetRange() const { return range; };
	
	UFUNCTION(BlueprintCallable)
	void SetRange(int val) { range = val; };
	
private:
	TArray<FGameplayAbilitySpecHandle>	GrantedAbilitySpecHandles;
};

