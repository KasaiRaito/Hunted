// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HuntedInteractable.generated.h"

UCLASS(Blueprintable)
class HUNTED_API AHuntedInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AHuntedInteractable();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	void Interact();
	
	virtual void Interact_Implementation();

};
