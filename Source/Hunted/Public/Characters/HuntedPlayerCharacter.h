// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/HuntedBaseCharacter.h"
#include "HuntedPlayerCharacter.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;

/**
 * 
 */
UCLASS()
class HUNTED_API AHuntedPlayerCharacter : public AHuntedBaseCharacter
{
	GENERATED_BODY()

public:
	AHuntedPlayerCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float SneakSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float CrouchSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float WalkSpeed;
	

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	
private:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Sneak(const FInputActionValue& Sneak);
	void Input_Sprint(const FInputActionValue& Sprint);
	void Input_Crouch(const FInputActionValue& Crouch);
	void Input_Snap(const FInputActionValue& Snap);
	
	void ProcessMovementInput(const FInputActionValue& InputActionValue);

	void Input_Look(const FInputActionValue& InputActionValue);

	bool IsSneak = false;
	bool IsSprint = false;
	bool IsCrouch = false;
	
#pragma endregion

#pragma region Functions
	void SnapFingers();
#pragma endregion
};
