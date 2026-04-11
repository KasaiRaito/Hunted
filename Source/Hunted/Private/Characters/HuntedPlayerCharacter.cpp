// KasaiRaito All Rights Reserved

#include "Characters/HuntedPlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/PlayerInputComponent.h"
#include "HuntedGameplayTags.h"
#include "DataAssets/StartUpData/DataAsset_PlayerStartUpData.h"
#include "Kismet/GameplayStatics.h"

#include "HuntedDebugHelper.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/HuntedPlayerGameplayAbility.h"
#include "Blueprint/UserWidget.h"

/** Components **/
#include "Components/Combat/PlayerCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Items/Inventory/HuntedInventoryItemBase.h"

AHuntedPlayerCharacter::AHuntedPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHuntedPlayerCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AHuntedPlayerCharacter::OnEndOverlap);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetCapsuleComponent());
	FollowCamera->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	
	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
	
	PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PlayerInventoryComponent"));
	
	UpdateStaticMeshList();
}

UPawnCombatComponent* AHuntedPlayerCharacter::GetPawnCombatComponent() const
{
	return PlayerCombatComponent;
	
}

UPawnUIComponent* AHuntedPlayerCharacter::GetPawnUIComponent() const
{
	return PlayerUIComponent;
}

UPlayerUIComponent* AHuntedPlayerCharacter::GetPlayerUIComponent() const
{
	return PlayerUIComponent;
}

void AHuntedPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GivenToAbilitySystemComponent(HuntedAbilitySystemComponent);
		}
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
	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Look,
		ETriggerEvent::Completed, this, &ThisClass::Input_LookStopped);

	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Crouch,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch);
	
	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Aim,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Aim);
	
	PlayerInputComponent->BindAbilityInputAction(InputConfigDataAsset,this,
		&ThisClass::Input_AbilityInputPressed,&ThisClass::Input_AbilityInputReleased);
}

void AHuntedPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (InventoryWidgetClass == nullptr)
	{
		Debug::Print(TEXT("Player InventoryWidgetClass is NULL"));
		return;
	}
	
	PlayerControllerComponent = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	InventoryWidget = CreateWidget(GetWorld(), InventoryWidgetClass);
	InventoryWidget->SetOwningPlayer(PlayerControllerComponent);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	PlayerInventoryComponent->SetItemsNum(PlayerInventoryComponent->GetColumns() * PlayerInventoryComponent->GetRows());
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

void AHuntedPlayerCharacter::Input_Aim(const FInputActionValue& Aim)
{
	if (bool InputVale = Aim.Get<bool>())
	{
		IsAiming = IsAimToggle ? !IsAiming : true;
		Debug::Print(TEXT("Press"));
	}
	else
	{
		IsAiming = IsAimToggle ? IsAiming : false;
		Debug::Print(TEXT("Release"));
	}
	
	Debug::Print(IsAiming ? TEXT("Aiming: TRUE") : TEXT("Aiming: FALSE"));
	LookSpeed = IsAiming ? AimSpeed : BaseSpeed;
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
	const FVector2D LookInput = InputActionValue.Get<FVector2D>();

	if (!Controller)
	{
		return;
	}

	if (!LookAcceleration || !CameraAcceleration)
	{
		AddControllerYawInput(LookInput.X * LookSpeed);
		AddControllerPitchInput(LookInput.Y * LookSpeed);
		return;
	}

	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	const float TargetYaw = (FMath::Abs(LookInput.X) > 0.05f) ? LookInput.X : 0.0f;
	const float TargetPitch = (FMath::Abs(LookInput.Y) > 0.05f) ? LookInput.Y : 0.0f;

	const float YawInterpSpeed =
		(FMath::Abs(TargetYaw) > FMath::Abs(CurrentYaw)) ? LookAcceleration : LookDeceleration;

	const float PitchInterpSpeed =
		(FMath::Abs(TargetPitch) > FMath::Abs(CurrentPitch)) ? LookAcceleration : LookDeceleration;

	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, YawInterpSpeed);
	CurrentPitch = FMath::FInterpTo(CurrentPitch, TargetPitch, DeltaTime, PitchInterpSpeed);

	AddControllerYawInput(CurrentYaw * LookSpeed);
	AddControllerPitchInput(CurrentPitch * LookSpeed);
	Debug::Print(TEXT("Input_Look"));
}

void AHuntedPlayerCharacter::Input_LookStopped(const FInputActionValue& InputActionValue)
{
	CurrentYaw = 0.f;
	CurrentPitch = 0.f;
	
	Debug::Print(TEXT("Input_Canceled"));
}

void AHuntedPlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	HuntedAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AHuntedPlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	HuntedAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
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

void AHuntedPlayerCharacter::OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AHuntedInventoryItemBase* Item = Cast<AHuntedInventoryItemBase>(OtherActor))
	{
		Item->SetIsInRangeOfPlayer( true );
		Debug::Print(TEXT("HuntedPlayerCharacter::OnBeginOverlap Item"), FColor::Yellow);
	}
}

void AHuntedPlayerCharacter::OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AHuntedInventoryItemBase* Item = Cast<AHuntedInventoryItemBase>(OtherActor))
	{
		Item->SetIsInRangeOfPlayer( false );
    	Debug::Print(TEXT("HuntedPlayerCharacter::OnEndOverlap Item"), FColor::Yellow);
		
    }
}