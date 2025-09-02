// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/HuntedBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "HuntedPlayerCharacter.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FActorMaterialBackup
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* Actor;

	UPROPERTY()
	TArray<UMaterialInterface*> Materials;
};

UCLASS()
class HUNTED_API AHuntedPlayerCharacter : public AHuntedBaseCharacter
{
	GENERATED_BODY()

public:
	AHuntedPlayerCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float SprintSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float SneakSpeed = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float CrouchSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float WalkSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Echo") 
	TArray<AActor*> StaticMeshActors;

	UPROPERTY()
	TArray<FActorMaterialBackup> OriginalActorMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
	UMaterialInterface* MyEchoMaterial;

	UFUNCTION(BlueprintCallable, Category="Echo")
	bool ReturnIsEcho();

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
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

	void Input_Echo(const FInputActionValue& Echo);
	
	void ProcessMovementInput(const FInputActionValue& InputActionValue);

	void Input_Look(const FInputActionValue& InputActionValue);

	bool IsSneak = false;
	bool IsSprint = false;
	bool IsCrouch = false;
	bool IsEcho = false;
	
#pragma endregion

#pragma region Functions
	void SnapFingers();

	void EnterEcho();

	void ExitEcho();

	void UpdateStaticMeshList();

	
	
#pragma endregion
};
