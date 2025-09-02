// KasaiRaito All Rights Reserved


#include "Characters/HuntedPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/PlayerInputComponent.h"
#include "HuntedGameplayTags.h"

#include "AbilitySystem/HuntedAbilitySystemComponent.h"

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

	UpdateStaticMeshList();
}

bool AHuntedPlayerCharacter::ReturnIsEcho()
{
	return IsEcho;
}

void AHuntedPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HuntedAbilitySystemComponent && HuntedAttributeSet)
	{
		const FString ASCText = FString::Printf(TEXT("Owner Actor: %s, AvatarActor: %s"),
			*HuntedAbilitySystemComponent->GetOwnerActor()->GetActorLabel(),
			*HuntedAbilitySystemComponent->GetAvatarActor()->GetActorLabel());

		Debug::Print(TEXT("Ability System Component VALID. ") + ASCText, FColor::Green);
		Debug::Print(TEXT("Abillity Set VAID"), FColor::Green);
	}
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

	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Snap,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Snap);

	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Echo,
		ETriggerEvent::Triggered,this, &ThisClass::Input_Echo);
}

void AHuntedPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AHuntedPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	// Process movement input
	ProcessMovementInput(InputActionValue);
}

void AHuntedPlayerCharacter::Input_Sneak(const FInputActionValue& Sneak)
{
	//Debug::Print(TEXT("HuntedPlayerCharacter::Input_Sneak"));
	IsSneak = Sneak.Get<bool>();
	if (IsSneak)
	{
		GetCharacterMovement()->MaxWalkSpeed = SneakSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AHuntedPlayerCharacter::Input_Sprint(const FInputActionValue& Sprint)
{
	//Debug::Print(TEXT("HuntedPlayerCharacter::Input_Sprint"));
	IsSprint = Sprint.Get<bool>();

	if (IsSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AHuntedPlayerCharacter::Input_Crouch(const FInputActionValue& Crouch)
{
	//Debug::Print(TEXT("HuntedPlayerCharacter::Input_Crouch"));
	IsCrouch = Crouch.Get<bool>();
	if (IsCrouch)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AHuntedPlayerCharacter::Input_Snap(const FInputActionValue& Snap)
{
	if (Snap.Get<bool>())
	{
		Debug::Print(TEXT("HuntedPlayerCharacter::Input_Snap"));
		SnapFingers();
	}
}

void AHuntedPlayerCharacter::Input_Echo(const FInputActionValue& Echo)
{
	if (Echo.Get<bool>())
	{
		Debug::Print(TEXT("HuntedPlayerCharacter::Input_Echo"));
		IsEcho = !IsEcho;
	}

	if (IsEcho)
	{
		EnterEcho();
	}
	else
	{
		ExitEcho();
	}
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

void AHuntedPlayerCharacter::SnapFingers()
{
	Debug::Print(TEXT("HuntedPlayerCharacter::SnapFingers"));
}

void AHuntedPlayerCharacter::EnterEcho()
{
	for (AActor* StaticMeshActor : StaticMeshActors)
	{
		if (!StaticMeshActor) continue;

		AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(StaticMeshActor);
		if (MeshActor)
		{
			UStaticMeshComponent* MeshComp = MeshActor->GetStaticMeshComponent();
			if (MeshComp && MyEchoMaterial)
			{
				int32 NumMats = MeshComp->GetNumMaterials();
				for (int32 i = 0; i < NumMats; i++)
				{
					MeshComp->SetMaterial(i, MyEchoMaterial);
				}
			}
		}
	}
}

void AHuntedPlayerCharacter::ExitEcho()
{
	for (const FActorMaterialBackup& Backup : OriginalActorMaterials)
	{
		if (!Backup.Actor) continue;

		AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(Backup.Actor);
		if (MeshActor)
		{
			UStaticMeshComponent* MeshComp = MeshActor->GetStaticMeshComponent();
			if (MeshComp)
			{
				for (int32 i = 0; i < Backup.Materials.Num(); i++)
				{
					MeshComp->SetMaterial(i, Backup.Materials[i]);
				}
			}
		}
	}
}

void AHuntedPlayerCharacter::UpdateStaticMeshList()
{
	StaticMeshActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), StaticMeshActors);

	OriginalActorMaterials.Empty();

	for (AActor* StaticMeshActor : StaticMeshActors)
	{
		if (!StaticMeshActor) continue;

		AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(StaticMeshActor);
		if (MeshActor)
		{
			UStaticMeshComponent* MeshComp = MeshActor->GetStaticMeshComponent();
			if (MeshComp)
			{
				FActorMaterialBackup Backup;
				Backup.Actor = MeshActor;

				int32 NumMats = MeshComp->GetNumMaterials();
				for (int32 i = 0; i < NumMats; i++)
				{
					Backup.Materials.Add(MeshComp->GetMaterial(i));
				}

				OriginalActorMaterials.Add(Backup);
			}
		}
	}
}


