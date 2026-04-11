// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/HuntedBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "GameplayTagContainer.h"
#include "Camera/CameraComponent.h"
#include "HuntedPlayerCharacter.generated.h"

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

protected:
	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	
	//~ Begin Inventory Component.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly ,Category = "UI")
	UUserWidget* InventoryWidget;
	
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
	AHuntedInventoryItemBase*  CachedItem;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void SetCachedItem(AHuntedInventoryItemBase* Item) { CachedItem = Item; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void ClearCachedItem() { CachedItem = nullptr; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE AHuntedInventoryItemBase* GetCachedItem() const { return CachedItem; }
	
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

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	

	bool IsSneak = false;
	bool IsSprint = false;
	bool IsCrouch = false;
	bool IsEcho = false;
	bool IsAiming = false;
	
	bool IsAimToggle = false;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool GetAimingState() const { return IsAiming; };

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetAimingState(bool bAimingState) { IsAiming = bAimingState; };
	
	
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
	
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
