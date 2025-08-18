// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/HuntedBaseCharacter.h"
#include "HuntedPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class HUNTED_API AHuntedPlayerCharacter : public AHuntedBaseCharacter
{
	GENERATED_BODY()

public:
	AHuntedPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	
private:
#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
#pragma endregion
};
