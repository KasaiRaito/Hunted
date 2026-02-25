// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/HuntedBaseCharacter.h"
#include "HuntedEnemyCharacter.generated.h"

class UEnemyCombatComponent;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class HUNTED_API AHuntedEnemyCharacter : public AHuntedBaseCharacter
{
	GENERATED_BODY()
	
public:
	AHuntedEnemyCharacter();
	
	//~ Begin IPawnCombatInterface Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface
	
	//~ Begin IPawnUIInterface Interface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
	//~ End IPawnUIInterface Interface
	
protected:
	// Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	// End APawn Interface
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;
	
	
private:
	void InitEnemyStartUpData();
	
public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const {return EnemyCombatComponent; };
};
