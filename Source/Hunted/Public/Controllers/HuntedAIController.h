// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HuntedAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
/**
 * 
 */
UCLASS()
class HUNTED_API AHuntedAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AHuntedAIController(const FObjectInitializer& ObjectInitializer);

	//~ Begin IGenericTeamInterface.
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamInterface.
		
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	UAIPerceptionComponent* EnemyPerceptionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	UAISenseConfig_Sight* AISenseConfig_Sight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	UAISenseConfig_Hearing* AISenseConfig_Hearing;
	
	UFUNCTION(BlueprintCallable, Category = Generated)
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Detour crown Avoidance Config")
	bool bEnableDetourCrowdAvoidance = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Detour crown Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;
	
	UPROPERTY(EditDefaultsOnly, Category = "Detour crown Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.0f;
};