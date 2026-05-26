// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/HuntedBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "GameplayTagContainer.h"
#include "Camera/CameraComponent.h"
#include "HuntedPlayerCharacter.generated.h"

class UHuntedWidgetBase;
class AHuntedInteractable;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
class UPlayerCombatComponent;
class UPlayerInventoryComponent;
class UPlayerUIComponent;
class AHuntedInventoryItemBase;

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
	
	//~ Begin IPawnCombatInterface Interface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface
	
	//~ Begin IPawnUIInterface Interface
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUIComponent() const override;
	//~ End IPawnUIInterface Interface
	
	//MOVEMENT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float SprintSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float SneakSpeed = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float CrouchSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float WalkSpeed = 250.f;
	
	//CAMERA
	UPROPERTY(EditAnywhere, Category="Camera")
	float BaseSpeed = 2.0f;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float AimSpeed = 0.5f;

	float LookSpeed = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	bool CameraAcceleration = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	bool IsAimToggle = false;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float LookAcceleration = 8.0f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float LookDeceleration = 12.0f;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	bool bUseLookSmoothing = true;
	
	UPROPERTY()
	float CurrentYaw = 0.f;
	UPROPERTY()
	float CurrentPitch = 0.f;
	
	// ECHO
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Echo") 
	TArray<AActor*> StaticMeshActors;

	UPROPERTY()
	TArray<FActorMaterialBackup> OriginalActorMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
	UMaterialInterface* MyEchoMaterial;

protected:
	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	
	//~ Begin Inventory Component.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;
	
	UPROPERTY(EditDefaultsOnly ,Category = "UI")
	UUserWidget* InventoryWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UUserWidget* PauseWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ItemWidgetClass;
	
	UPROPERTY(EditDefaultsOnly ,Category = "UI")
	UUserWidget* ItemWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	AActor* ItemToAdd;
	//~ End Inventory Component
	
private:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FVector GetFollowCameraLocation() const { return FollowCamera->GetComponentLocation(); }
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	FVector GetFollowCameraForward() const { return FollowCamera->GetForwardVector(); }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UPlayerCombatComponent* PlayerCombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	APlayerController* PlayerControllerComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UPlayerUIComponent* PlayerUIComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UPlayerInventoryComponent* PlayerInventoryComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	AHuntedInteractable*  CachedItem;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void SetCachedItem(AHuntedInteractable* Item) { CachedItem = Item; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void ClearCachedItem() { CachedItem = nullptr; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE AHuntedInteractable* GetCachedItem() const { return CachedItem; }
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool GetAimingState() const { return IsAiming; };

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetAimingState(bool bAimingState) { IsAiming = bAimingState; };
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool GetHaveGun() const { return HaveGun; };
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetHaveGun(bool bHaveGun) { HaveGun = bHaveGun; };
	
private:
	
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Sneak(const FInputActionValue& Sneak);
	void Input_Sprint(const FInputActionValue& Sprint);
	void Input_Crouch(const FInputActionValue& Crouch);
	void Input_Aim(const FInputActionValue& Aim);
	
	void ProcessMovementInput(const FInputActionValue& InputActionValue);

	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_LookStopped(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	

	bool IsSneak = false;
	bool IsSprint = false;
	bool IsCrouch = false;
	bool IsEcho = false;
	bool IsAiming = false;
	bool HaveGun = false;
	
#pragma endregion

#pragma region Functions
	void SnapFingers();

	void UpdateStaticMeshList();	
#pragma endregion

public:	
	UFUNCTION(BlueprintCallable, Category = "Echo")
	void EnterEcho();

	UFUNCTION(BlueprintCallable, Category = "Echo")
	void ExitEcho();
	
	UFUNCTION(BlueprintCallable, Category="Echo")
	FORCEINLINE bool ReturnIsEcho() const { return IsEcho; };
	
	UFUNCTION(BlueprintCallable, Category="Echo")
	FORCEINLINE void SetIsEcho(bool val) { IsEcho = val; };
	
	FORCEINLINE UPlayerCombatComponent* GetPlayerCombatComponent()const { return PlayerCombatComponent; }
	FORCEINLINE UPlayerInventoryComponent* GetPlayerInventoryComponent()const { return PlayerInventoryComponent; }
	
	FORCEINLINE TSubclassOf<UUserWidget> GetItemWidgetClass()const { return ItemWidgetClass; }
	FORCEINLINE UUserWidget* GetItemWidget()const { return ItemWidget; }
	FORCEINLINE void SetItemWidget(UUserWidget* widget) { ItemWidget = widget; }
	
	UFUNCTION(BlueprintCallable, Category = "Invewntory")
	FORCEINLINE UUserWidget* GetInventoryWidget() { return InventoryWidget; }
	
	UFUNCTION(BlueprintCallable, Category = "Pause")
	FORCEINLINE UUserWidget* GetPauseWidget() { return PauseWidget; }
	
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
