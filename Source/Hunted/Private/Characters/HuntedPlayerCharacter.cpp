// KasaiRaito All Rights Reserved

#include "Characters/HuntedPlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputCoreTypes.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/PlayerInputComponent.h"
#include "HuntedGameplayTags.h"
#include "DataAssets/StartUpData/DataAsset_PlayerStartUpData.h"
#include "Kismet/GameplayStatics.h"

#include "HuntedDebugHelper.h"
#include "AbilitySystem/HuntedAbilitySystemComponent.h"
#include "AbilitySystem/HuntedAttributeSet.h"
#include "AbilitySystem/Abilities/HuntedPlayerGameplayAbility.h"
#include "AnimInstances/Player/HuntedPlayerLinkedAnimLayer.h"
#include "Blueprint/UserWidget.h"

/** Components **/
#include "Components/Combat/PlayerCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "ContextualAnimSceneActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Items/Inventory/HuntedInventoryItemBase.h"
#include "Items/Weapons/HuntedPlayerWeaponBase.h"

namespace
{
	constexpr float PostContextControlRotationSyncTime = 0.25f;
}

AHuntedPlayerCharacter::AHuntedPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHuntedPlayerCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AHuntedPlayerCharacter::OnEndOverlap);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	
	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
	
	PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PlayerInventoryComponent"));

	WeaponCycleInputAction = CreateDefaultSubobject<UInputAction>(TEXT("WeaponCycleInputAction"));
	WeaponCycleInputAction->ValueType = EInputActionValueType::Axis1D;

	WeaponCycleMappingContext =
		CreateDefaultSubobject<UInputMappingContext>(TEXT("WeaponCycleMappingContext"));
	WeaponCycleMappingContext->MapKey(WeaponCycleInputAction, EKeys::MouseWheelAxis);

	// These entries keep the native switch code aligned with the existing Gun and Knife Blueprint abilities.
	FHuntedWeaponSwitchEntry KnifeEntry;
	KnifeEntry.WeaponTag = HuntedGameplayTags::Player_Weapon_BasicKnife;
	KnifeEntry.EquipInputTag = HuntedGameplayTags::InputTag_Equip_Knife;
	KnifeEntry.UnequipInputTag = HuntedGameplayTags::InputTag_Unequip_Knife;
	KnifeEntry.UnequippedSocketName = TEXT("Weapon_RightThighSocket");
	WeaponCycleEntries.Add(KnifeEntry);

	FHuntedWeaponSwitchEntry GunEntry;
	GunEntry.WeaponTag = HuntedGameplayTags::Player_Weapon_Gun;
	GunEntry.EquipInputTag = HuntedGameplayTags::InputTag_Equip_Gun;
	GunEntry.UnequipInputTag = HuntedGameplayTags::InputTag_Unequip_Gun;
	GunEntry.UnequippedSocketName = TEXT("Weapon_PelvisSocket");
	WeaponCycleEntries.Add(GunEntry);

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

	BindSanityChangedDelegate();
	BindHealthChangedDelegate();
}

void AHuntedPlayerCharacter::SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent)
{
	if (!InputConfigDataAsset)
	{
		// Missing input assets should disable binding, not crash after level start.
		Debug::Print(TEXT("InputConfigDataAsset is not assigned"), FColor::Red);
		return;
	}
	
	APlayerController* PlayerController = GetController<APlayerController>();
	if (!PlayerController)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	if (!Subsystem)
	{
		return;
	}

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	Subsystem->AddMappingContext(WeaponCycleMappingContext, 1);

	UPlayerInputComponent* PlayerInputComponent = Cast<UPlayerInputComponent>(InPlayerInputComponent);
	if (!PlayerInputComponent)
	{
		return;
	}

	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, HuntedGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered, this, &ThisClass::Input_Move);

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

	// This native Enhanced Input action avoids requiring another content asset just for the mouse wheel.
	PlayerInputComponent->BindAction(
		WeaponCycleInputAction, ETriggerEvent::Triggered, this, &ThisClass::Input_CycleWeapon);
}

void AHuntedPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	BindHealthChangedDelegate();
	
	ContextualAnimSceneActorComponent = FindComponentByClass<UContextualAnimSceneActorComponent>();
	if (ContextualAnimSceneActorComponent)
	{
		ContextualAnimSceneActorComponent->OnJoinedSceneDelegate.AddUniqueDynamic(
			this, &ThisClass::HandleContextualAnimSceneJoined);
		ContextualAnimSceneActorComponent->OnLeftSceneDelegate.AddUniqueDynamic(
			this, &ThisClass::HandleContextualAnimSceneLeft);
	}
	
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
	
	PauseWidget = CreateWidget(GetWorld(), PauseWidgetClass);
	PauseWidget->SetOwningPlayer(PlayerControllerComponent);
	PauseWidget->AddToViewport();
	PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	
	PlayerInventoryComponent->SetItemsNum(PlayerInventoryComponent->GetColumns() * PlayerInventoryComponent->GetRows());
}

void AHuntedPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HuntedAbilitySystemComponent && SanityChangedDelegateHandle.IsValid())
	{
		HuntedAbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(UHuntedAttributeSet::GetCurrentSanityAttribute())
			.Remove(SanityChangedDelegateHandle);
		SanityChangedDelegateHandle.Reset();
	}

	if (HuntedAbilitySystemComponent && HealthChangedDelegateHandle.IsValid())
	{
		HuntedAbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(UHuntedAttributeSet::GetCurrentHealthAttribute())
			.Remove(HealthChangedDelegateHandle);
		HealthChangedDelegateHandle.Reset();
	}

	if (ContextualAnimSceneActorComponent)
	{
		ContextualAnimSceneActorComponent->OnJoinedSceneDelegate.RemoveDynamic(
			this, &ThisClass::HandleContextualAnimSceneJoined);
		ContextualAnimSceneActorComponent->OnLeftSceneDelegate.RemoveDynamic(
			this, &ThisClass::HandleContextualAnimSceneLeft);
	}

	Super::EndPlay(EndPlayReason);
}

void AHuntedPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsContextualAnimSceneActive())
	{
		if (ControlRotation || bUseControllerRotationYaw)
		{
			ApplyControlRotationState(false);
		}

		SyncControlRotationToActorYaw();
		return;
	}

	if (bPendingEnableControlRotation)
	{
		bPendingEnableControlRotation = false;
		ApplyControlRotationState(true);
		PendingControlRotationSyncTime = PostContextControlRotationSyncTime;
	}

	if (!ControlRotation)
	{
		SyncControlRotationToActorYaw();
		return;
	}

	if (PendingControlRotationSyncTime > 0.f)
	{
		SyncControlRotationToActorYaw();
		PendingControlRotationSyncTime -= DeltaSeconds;
	}

	if (ControlRotation && PendingControlRotationSyncTime <= 0.f)
	{
		SetActorTickEnabled(false);
	}
}

void AHuntedPlayerCharacter::SetControlRotationEnabled(bool bShouldControlRotation)
{
	CurrentYaw = 0.f;
	CurrentPitch = 0.f;
	
	const FString ControlRotationValue = bShouldControlRotation ? TEXT("True") : TEXT("False");
	Debug::Print(TEXT("ControlRotation Value: ") + ControlRotationValue);

	if (!bShouldControlRotation)
	{
		bPendingEnableControlRotation = IsContextualAnimSceneActive() && (ControlRotation || bPendingEnableControlRotation);
		PendingControlRotationSyncTime = 0.f;
		ApplyControlRotationState(false);
		SetActorTickEnabled(true);
		return;
	}

	if (IsContextualAnimSceneActive())
	{
		bPendingEnableControlRotation = true;
		ApplyControlRotationState(false);
		SetActorTickEnabled(true);
		return;
	}

	bPendingEnableControlRotation = false;
	ApplyControlRotationState(true);
	PendingControlRotationSyncTime = PostContextControlRotationSyncTime;
	SetActorTickEnabled(true);
}

void AHuntedPlayerCharacter::ApplyControlRotationState(bool bShouldControlRotation)
{
	ControlRotation = bShouldControlRotation;
	SyncControlRotationToActorYaw();

	bUseControllerRotationYaw = ControlRotation;
	if (FirstPersonCamera)
	{
		FirstPersonCamera->bUsePawnControlRotation = true;
	}

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

bool AHuntedPlayerCharacter::IsContextualAnimSceneActive() const
{
	return ContextualAnimSceneActorComponent && ContextualAnimSceneActorComponent->IsInActiveScene();
}

void AHuntedPlayerCharacter::SyncControlRotationToActorYaw()
{
	if (!Controller)
	{
		return;
	}

	FRotator NewControlRotation = Controller->GetControlRotation();
	NewControlRotation.Yaw = GetActorRotation().Yaw;
	NewControlRotation.Roll = 0.f;
	Controller->SetControlRotation(NewControlRotation);
}

void AHuntedPlayerCharacter::HandleContextualAnimSceneJoined(UContextualAnimSceneActorComponent* SceneActorComponent)
{
	bPendingEnableControlRotation = bPendingEnableControlRotation || ControlRotation;
	PendingControlRotationSyncTime = 0.f;
	ApplyControlRotationState(false);
	SetActorTickEnabled(true);
}

void AHuntedPlayerCharacter::HandleContextualAnimSceneLeft(UContextualAnimSceneActorComponent* SceneActorComponent)
{
	SyncControlRotationToActorYaw();

	if (bPendingEnableControlRotation)
	{
		bPendingEnableControlRotation = false;
		ApplyControlRotationState(true);
	}

	PendingControlRotationSyncTime = PostContextControlRotationSyncTime;
	SetActorTickEnabled(true);
}

void AHuntedPlayerCharacter::ApplyUseEffect(UHuntedAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	if (!IsValid(AbilitySystemComponent) || !EchoUseGameplayEffectClass)
	{
		Debug::Print(TEXT("Cannot apply Echo sanity drain"), FColor::Red);
		return;
	}

	if (EchoRegenEffectHandle.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(EchoRegenEffectHandle);
		EchoRegenEffectHandle.Invalidate();
	}

	if (EchoUseEffectHandle.IsValid())
	{
		if (AbilitySystemComponent->GetActiveGameplayEffect(EchoUseEffectHandle))
		{
			return;
		}

		EchoUseEffectHandle.Invalidate();
	}

	FGameplayEffectContextHandle EffectContext =
		AbilitySystemComponent->MakeEffectContext();

	EffectContext.AddSourceObject(this);

	const UGameplayEffect* Effect =
		EchoUseGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	EchoUseEffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(
		Effect,
		FMath::Max(1, ApplyLevel),
		EffectContext);
}

void AHuntedPlayerCharacter::ApplyRegenEffect(UHuntedAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	if (!IsValid(AbilitySystemComponent) || !EchoRegenGameplayEffectClass)
	{
		Debug::Print(TEXT("Cannot apply Echo sanity regeneration"), FColor::Red);
		return;
	}

	if (EchoUseEffectHandle.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(EchoUseEffectHandle);
		EchoUseEffectHandle.Invalidate();
	}

	if (EchoRegenEffectHandle.IsValid())
	{
		if (AbilitySystemComponent->GetActiveGameplayEffect(EchoRegenEffectHandle))
		{
			return;
		}

		EchoRegenEffectHandle.Invalidate();
	}

	FGameplayEffectContextHandle EffectContext =
		AbilitySystemComponent->MakeEffectContext();

	EffectContext.AddSourceObject(this);

	const UGameplayEffect* Effect =
		EchoRegenGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	EchoRegenEffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(
		Effect,
		FMath::Max(1, ApplyLevel),
		EffectContext);
}

void AHuntedPlayerCharacter::BindSanityChangedDelegate()
{
	if (!HuntedAbilitySystemComponent || SanityChangedDelegateHandle.IsValid())
	{
		return;
	}

	SanityChangedDelegateHandle = HuntedAbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(UHuntedAttributeSet::GetCurrentSanityAttribute())
		.AddUObject(this, &ThisClass::HandleCurrentSanityChanged);
}

void AHuntedPlayerCharacter::HandleCurrentSanityChanged(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.OldValue > 0.f && ChangeData.NewValue <= 0.f)
	{
		ActivateZeroSanityAbility();
	}
}

void AHuntedPlayerCharacter::BindHealthChangedDelegate()
{
	if (!HuntedAbilitySystemComponent || HealthChangedDelegateHandle.IsValid())
	{
		return;
	}

	HealthChangedDelegateHandle = HuntedAbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(UHuntedAttributeSet::GetCurrentHealthAttribute())
		.AddUObject(this, &ThisClass::HandleCurrentHealthChanged);
}

void AHuntedPlayerCharacter::HandleCurrentHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.OldValue > 0.f && ChangeData.NewValue <= 0.f)
	{
		ShowDeathWidget();
	}
}

bool AHuntedPlayerCharacter::ActivateZeroSanityAbility()
{
	if (!HuntedAbilitySystemComponent || !ZeroSanityGameplayAbilityClass)
	{
		return false;
	}

	return HuntedAbilitySystemComponent->TryActivateAbilityByClass(ZeroSanityGameplayAbilityClass);
}

void AHuntedPlayerCharacter::ShowDeathWidget()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (!DeathWidgetClass)
	{
		Debug::Print(TEXT("DeathWidgetClass is not assigned on HuntedPlayerCharacter"), FColor::Red);
		return;
	}

	APlayerController* OwningPlayerController = Cast<APlayerController>(GetController());
	if (!OwningPlayerController)
	{
		return;
	}

	if (!DeathWidget)
	{
		DeathWidget = CreateWidget<UUserWidget>(OwningPlayerController, DeathWidgetClass);
	}

	if (!DeathWidget)
	{
		return;
	}

	if (!DeathWidget->IsInViewport())
	{
		DeathWidget->AddToViewport(DeathWidgetZOrder);
	}

	DeathWidget->SetVisibility(ESlateVisibility::Visible);

	if (bSetUIOnlyInputModeOnDeath)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(DeathWidget->TakeWidget());
		OwningPlayerController->SetInputMode(InputMode);
		OwningPlayerController->bShowMouseCursor = true;
	}

	BP_OnDeathWidgetShown(DeathWidget);
}

void AHuntedPlayerCharacter::HideDeathWidget()
{
	if (!DeathWidget)
	{
		return;
	}

	UUserWidget* HiddenDeathWidget = DeathWidget;
	DeathWidget->RemoveFromParent();

	if (bSetUIOnlyInputModeOnDeath)
	{
		if (APlayerController* OwningPlayerController = Cast<APlayerController>(GetController()))
		{
			FInputModeGameOnly InputMode;
			OwningPlayerController->SetInputMode(InputMode);
			OwningPlayerController->bShowMouseCursor = false;
		}
	}

	BP_OnDeathWidgetHidden(HiddenDeathWidget);
}

void AHuntedPlayerCharacter::ShowVictoryWidget()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (!VictoryWidgetClass)
	{
		Debug::Print(TEXT("VictoryWidgetClass is not assigned on HuntedPlayerCharacter"), FColor::Red);
		return;
	}

	APlayerController* OwningPlayerController = Cast<APlayerController>(GetController());
	if (!OwningPlayerController)
	{
		return;
	}

	if (!VictoryWidget)
	{
		VictoryWidget = CreateWidget<UUserWidget>(OwningPlayerController, VictoryWidgetClass);
	}

	if (!VictoryWidget)
	{
		return;
	}

	if (!VictoryWidget->IsInViewport())
	{
		VictoryWidget->AddToViewport(VictoryWidgetZOrder);
	}

	VictoryWidget->SetVisibility(ESlateVisibility::Visible);

	if (bSetUIOnlyInputModeOnVictory)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(VictoryWidget->TakeWidget());
		OwningPlayerController->SetInputMode(InputMode);
		OwningPlayerController->bShowMouseCursor = true;
	}

	BP_OnVictoryWidgetShown(VictoryWidget);
}

void AHuntedPlayerCharacter::HideVictoryWidget()
{
	if (!VictoryWidget)
	{
		return;
	}

	UUserWidget* HiddenVictoryWidget = VictoryWidget;
	VictoryWidget->RemoveFromParent();

	if (bSetUIOnlyInputModeOnVictory)
	{
		if (APlayerController* OwningPlayerController = Cast<APlayerController>(GetController()))
		{
			FInputModeGameOnly InputMode;
			OwningPlayerController->SetInputMode(InputMode);
			OwningPlayerController->bShowMouseCursor = false;
		}
	}

	BP_OnVictoryWidgetHidden(HiddenVictoryWidget);
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
	
	/*
	if (IsAiming)
	{
		
	}
	
	else
	{
		
	}
	*/
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
	if (!Controller || !ControlRotation)
	{
		CurrentYaw = 0.f;
		CurrentPitch = 0.f;
		return;
	}

	const FVector2D LookInput = InputActionValue.Get<FVector2D>();

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
	//Debug::Print(TEXT("Input_Look"));
}

void AHuntedPlayerCharacter::Input_LookStopped(const FInputActionValue& InputActionValue)
{
	CurrentYaw = 0.f;
	CurrentPitch = 0.f;
	
	//Debug::Print(TEXT("Input_Canceled"));
}

void AHuntedPlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	if (!HuntedAbilitySystemComponent)
	{
		return;
	}

	if (const FHuntedWeaponSwitchEntry* WeaponEntry = FindWeaponEntryByEquipInput(InInputTag))
	{
		RequestEquipWeapon(*WeaponEntry);
		return;
	}

	if (const FHuntedWeaponSwitchEntry* WeaponEntry = FindWeaponEntryByUnequipInput(InInputTag))
	{
		const bool bStartedUnequip = HuntedAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
		if (bStartedUnequip && PlayerCombatComponent->CurrentEquippedWeaponTag == WeaponEntry->WeaponTag)
		{
			// Clear this immediately so the same weapon can be equipped again after its montage finishes.
			PendingUnequippedWeaponTag = WeaponEntry->WeaponTag;
			PlayerCombatComponent->CurrentEquippedWeaponTag = FGameplayTag();
			IsAiming = false;
		}
		return;
	}

	HuntedAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AHuntedPlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	if (HuntedAbilitySystemComponent)
	{
		HuntedAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
	}
}

void AHuntedPlayerCharacter::Input_CycleWeapon(const FInputActionValue& InputActionValue)
{
	const float WheelValue = InputActionValue.Get<float>();
	if (!FMath::IsNearlyZero(WheelValue))
	{
		CycleWeapon(WheelValue > 0.f ? 1 : -1);
	}
}

void AHuntedPlayerCharacter::CycleWeapon(int32 Direction)
{
	if (!PlayerCombatComponent || Direction == 0)
	{
		return;
	}

	TArray<int32> AvailableEntryIndexes;
	for (int32 EntryIndex = 0; EntryIndex < WeaponCycleEntries.Num(); ++EntryIndex)
	{
		const FHuntedWeaponSwitchEntry& Entry = WeaponCycleEntries[EntryIndex];
		if (IsValid(PlayerCombatComponent->GetPlayerCarriedWeaponByTag(Entry.WeaponTag)))
		{
			AvailableEntryIndexes.Add(EntryIndex);
		}
	}

	if (AvailableEntryIndexes.IsEmpty())
	{
		return;
	}

	const FGameplayTag CurrentWeaponTag = PlayerCombatComponent->CurrentEquippedWeaponTag;
	int32 CurrentAvailableIndex = INDEX_NONE;
	for (int32 AvailableIndex = 0; AvailableIndex < AvailableEntryIndexes.Num(); ++AvailableIndex)
	{
		if (WeaponCycleEntries[AvailableEntryIndexes[AvailableIndex]].WeaponTag == CurrentWeaponTag)
		{
			CurrentAvailableIndex = AvailableIndex;
			break;
		}
	}

	int32 NextAvailableIndex;
	if (CurrentAvailableIndex == INDEX_NONE)
	{
		NextAvailableIndex = Direction > 0 ? 0 : AvailableEntryIndexes.Num() - 1;
	}
	else
	{
		NextAvailableIndex = (CurrentAvailableIndex + FMath::Sign(Direction) + AvailableEntryIndexes.Num())
			% AvailableEntryIndexes.Num();
	}

	RequestEquipWeapon(WeaponCycleEntries[AvailableEntryIndexes[NextAvailableIndex]]);
}

bool AHuntedPlayerCharacter::RequestEquipWeapon(const FHuntedWeaponSwitchEntry& WeaponEntry)
{
	if (!PlayerCombatComponent || !HuntedAbilitySystemComponent ||
		!WeaponEntry.WeaponTag.IsValid() || !WeaponEntry.EquipInputTag.IsValid())
	{
		return false;
	}

	AHuntedPlayerWeaponBase* WeaponToEquip =
		PlayerCombatComponent->GetPlayerCarriedWeaponByTag(WeaponEntry.WeaponTag);
	if (!IsValid(WeaponToEquip))
	{
		return false;
	}

	if (PlayerCombatComponent->CurrentEquippedWeaponTag == WeaponEntry.WeaponTag)
	{
		return true;
	}

	// Stop a partially completed equip montage before it can grant inputs after another weapon was selected.
	for (const FHuntedWeaponSwitchEntry& Entry : WeaponCycleEntries)
	{
		HuntedAbilitySystemComponent->CancelAbilitiesByInputTag(Entry.EquipInputTag);
		HuntedAbilitySystemComponent->CancelAbilitiesByInputTag(Entry.UnequipInputTag);
	}

	if (!PlayerCombatComponent->CurrentEquippedWeaponTag.IsValid() &&
		PendingUnequippedWeaponTag.IsValid())
	{
		// If scrolling interrupts an unequip montage, finish that weapon's cleanup synchronously.
		PlayerCombatComponent->CurrentEquippedWeaponTag = PendingUnequippedWeaponTag;
	}
	PendingUnequippedWeaponTag = FGameplayTag();

	CacheWeaponUnequippedSocket(WeaponEntry);
	DeactivateCurrentWeaponForSwitch();

	WeaponToEquip->SetActorHiddenInGame(false);
	PlayerCombatComponent->CurrentEquippedWeaponTag = WeaponEntry.WeaponTag;

	if (!HuntedAbilitySystemComponent->OnAbilityInputPressed(WeaponEntry.EquipInputTag))
	{
		PlayerCombatComponent->CurrentEquippedWeaponTag = FGameplayTag();
		return false;
	}

	return true;
}

void AHuntedPlayerCharacter::DeactivateCurrentWeaponForSwitch()
{
	if (!PlayerCombatComponent || !HuntedAbilitySystemComponent)
	{
		return;
	}

	const FGameplayTag PreviousWeaponTag = PlayerCombatComponent->CurrentEquippedWeaponTag;
	if (!PreviousWeaponTag.IsValid())
	{
		return;
	}

	AHuntedPlayerWeaponBase* PreviousWeapon =
		PlayerCombatComponent->GetPlayerCarriedWeaponByTag(PreviousWeaponTag);
	if (!IsValid(PreviousWeapon))
	{
		PlayerCombatComponent->CurrentEquippedWeaponTag = FGameplayTag();
		return;
	}

	// A weapon loses every runtime-owned resource before another one can acquire combat input.
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles =
		PreviousWeapon->GetGrantedAbilitySpecHandle();
	HuntedAbilitySystemComponent->RemoveGrantedPlayerWeaponAbilities(GrantedAbilityHandles);
	PreviousWeapon->AssignGratedAbilitySpecHandles(GrantedAbilityHandles);

	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = GetEnhancedInputSubsystem())
	{
		if (UInputMappingContext* MappingContext =
			PreviousWeapon->PlayerWeaponData.WeaponInputMappingContext)
		{
			InputSubsystem->RemoveMappingContext(MappingContext);
		}
	}

	if (PreviousWeapon->PlayerWeaponData.WeaponAnimLayerToLink)
	{
		GetMesh()->UnlinkAnimClassLayers(
			TSubclassOf<UAnimInstance>(PreviousWeapon->PlayerWeaponData.WeaponAnimLayerToLink.Get()));
	}

	PlayerCombatComponent->ToggleWeaponCollision(false);
	IsAiming = false;

	const FHuntedWeaponSwitchEntry* PreviousEntry = FindWeaponEntryByWeaponTag(PreviousWeaponTag);
	const FName* CachedSocket = WeaponUnequippedSocketCache.Find(PreviousWeaponTag);
	const FName UnequippedSocket = CachedSocket
		? *CachedSocket
		: (PreviousEntry ? PreviousEntry->UnequippedSocketName : NAME_None);

	if (!UnequippedSocket.IsNone() && GetMesh()->DoesSocketExist(UnequippedSocket))
	{
		PreviousWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			UnequippedSocket);
		PreviousWeapon->SetActorHiddenInGame(false);
	}
	else
	{
		// A missing custom socket must not leave the old weapon visible in the equipped hand.
		PreviousWeapon->SetActorHiddenInGame(true);
	}

	PlayerCombatComponent->CurrentEquippedWeaponTag = FGameplayTag();
}

void AHuntedPlayerCharacter::CacheWeaponUnequippedSocket(const FHuntedWeaponSwitchEntry& WeaponEntry)
{
	if (WeaponUnequippedSocketCache.Contains(WeaponEntry.WeaponTag) || !PlayerCombatComponent)
	{
		return;
	}

	FName SocketName = WeaponEntry.UnequippedSocketName;
	if (const AHuntedPlayerWeaponBase* Weapon =
		PlayerCombatComponent->GetPlayerCarriedWeaponByTag(WeaponEntry.WeaponTag))
	{
		if (const USceneComponent* WeaponRootComponent = Weapon->GetRootComponent())
		{
			const FName CurrentSocketName = WeaponRootComponent->GetAttachSocketName();
			if (!CurrentSocketName.IsNone())
			{
				SocketName = CurrentSocketName;
			}
		}
	}

	WeaponUnequippedSocketCache.Add(WeaponEntry.WeaponTag, SocketName);
}

const FHuntedWeaponSwitchEntry* AHuntedPlayerCharacter::FindWeaponEntryByEquipInput(
	FGameplayTag InputTag) const
{
	return WeaponCycleEntries.FindByPredicate(
		[InputTag](const FHuntedWeaponSwitchEntry& Entry)
		{
			return Entry.EquipInputTag == InputTag;
		});
}

const FHuntedWeaponSwitchEntry* AHuntedPlayerCharacter::FindWeaponEntryByUnequipInput(
	FGameplayTag InputTag) const
{
	return WeaponCycleEntries.FindByPredicate(
		[InputTag](const FHuntedWeaponSwitchEntry& Entry)
		{
			return Entry.UnequipInputTag == InputTag;
		});
}

const FHuntedWeaponSwitchEntry* AHuntedPlayerCharacter::FindWeaponEntryByWeaponTag(
	FGameplayTag WeaponTag) const
{
	return WeaponCycleEntries.FindByPredicate(
		[WeaponTag](const FHuntedWeaponSwitchEntry& Entry)
		{
			return Entry.WeaponTag == WeaponTag;
		});
}

UEnhancedInputLocalPlayerSubsystem* AHuntedPlayerCharacter::GetEnhancedInputSubsystem() const
{
	const APlayerController* PlayerController = GetController<APlayerController>();
	const ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;
	return LocalPlayer
		? ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer)
		: nullptr;
}

void AHuntedPlayerCharacter::EnterEcho()
{
	/**
	 *Legacy Solution 
	 *Overided in BP
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
	**/
}

void AHuntedPlayerCharacter::ExitEcho()
{
	/**
	 *Legacy Solution
	 *Overided in BP
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
	**/
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
