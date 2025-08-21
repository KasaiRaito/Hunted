// KasaiRaito All Rights Reserved


#include "Characters/HuntedPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/PlayerInputComponent.h"
#include "HuntedGameplayTags.h"

#include "HuntedDebugHelper.h"

AHuntedPlayerCharacter::AHuntedPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//I REMOVED CAMERA BOOM, TO MAKE IT FIRST PERSON

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetRootComponent());
	FollowCamera->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	
}

void AHuntedPlayerCharacter::SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("InputConfigDataAsset is not ASSIGNED"))
	
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UPlayerInputComponent* PlayerInputComponent = CastChecked<UPlayerInputComponent>(InPlayerInputComponent);

	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	
	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Sneak,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Sneak);

	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Sprint,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Sprint);

	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Look,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Crouch,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch);
}

void AHuntedPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print(TEXT("HuntedPlayerCharacter::BeginPlay"));
}

void AHuntedPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	if (IsSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else if (IsSneak)
	{
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	}
	else if (IsCrouch)
	{
		GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	}

	// Process movement input
	ProcessMovementInput(InputActionValue);
}

void AHuntedPlayerCharacter::Input_Sneak(const FInputActionValue& Sneak)
{
	//Debug::Print(TEXT("HuntedPlayerCharacter::Input_Sneak"));
	IsSneak = Sneak.Get<bool>();
}

void AHuntedPlayerCharacter::Input_Sprint(const FInputActionValue& Sprint)
{
	//Debug::Print(TEXT("HuntedPlayerCharacter::Input_Sprint"));
	IsSprint = Sprint.Get<bool>();
}

void AHuntedPlayerCharacter::Input_Crouch(const FInputActionValue& Crouch)
{
	//Debug::Print(TEXT("HuntedPlayerCharacter::Input_Crouch"));
	IsCrouch = Crouch.Get<bool>();
}

void AHuntedPlayerCharacter::ProcessMovementInput(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotator.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotator.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AHuntedPlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
