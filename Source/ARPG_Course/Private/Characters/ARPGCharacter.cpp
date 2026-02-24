// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ARPGCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

AARPGCharacter::AARPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputActionContext, 0);
		}
	}
}

void AARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AARPGCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AARPGCharacter::Lookout);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyPressedAction, ETriggerEvent::Triggered, this, &AARPGCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AARPGCharacter::Attack);
	}
}

void AARPGCharacter::SetEnableBoxCollision(ECollisionEnabled::Type BoxCollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBoxComponent())
	{
		EquippedWeapon->GetWeaponBoxComponent()->SetCollisionEnabled(BoxCollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void AARPGCharacter::Move(const FInputActionValue& Value)
{
	//if (ActionState != EActionState::EAS_Unoccupied) return;
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Find out forward and right vector based on controller rotation
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	// Forward direction (X axis)
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// Right direction (Y axis)
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Forward / Backward
	if (CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon)
	{
		MovementVector = MovementVector / 3;
	}
	AddMovementInput(ForwardDirection, MovementVector.Y);
	
	// Right / Left (A/D)
	AddMovementInput(RightDirection, MovementVector.X);
}

void AARPGCharacter::Lookout(const FInputActionValue& Value)
{
	const FVector2D LookoutVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookoutVector.X);
	AddControllerPitchInput(LookoutVector.Y);
}

void AARPGCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon && CharacterState == ECharacterState::ECS_Unequipped)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("HandGrip_R"));
		SetOverlappingItem(nullptr);
		EquippedWeapon = OverlappingWeapon;
		SetCharacterState();
	}
	else
	{
		if (CanDisarm())
		{
			PlayMontage(ArmDisarmMontage, FName("Disarm"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_Arming;
		}
		else if (CanArm())
		{
			PlayMontage(ArmDisarmMontage, FName("Arm"));
			SetCharacterState();
			ActionState = EActionState::EAS_Arming;
		}
	}
}

void AARPGCharacter::Attack()
{
	if (CanAttack())
	{
		if (CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon)
		{
			PlayMontage(AttackMontage, FName("Attack_2HWeapon"));
		}
		else
		{
			PlayMontage(AttackMontage);
		}
		ActionState = EActionState::EAS_Attacking;
	}
}

void AARPGCharacter::PlayMontage(UAnimMontage* AnimMontageToPlay, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimMontageToPlay)
	{
		AnimInstance->Montage_Play(AnimMontageToPlay);
		if (SectionName != "")
		{
			AnimInstance->Montage_JumpToSection(SectionName, AnimMontageToPlay);
		}
		else
		{
			//Check number of sections in AnimMontage to generate random index
			const int32 NumberOfSections = AnimMontageToPlay->GetNumSections() - 1;
			const int32 RandomSectionIndex = FMath::RandRange(0, NumberOfSections);
			//Get Section Name using random index
			const FName RandomSectionName = AnimMontageToPlay->GetSectionName(RandomSectionIndex);
			AnimInstance->Montage_JumpToSection(RandomSectionName, AnimMontageToPlay);
		}
	}
}

void AARPGCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AARPGCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool AARPGCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool AARPGCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void AARPGCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Equip(GetMesh(), FName("SpineWeaponSocket"));
	}
}

void AARPGCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Equip(GetMesh(), FName("HandGrip_R"));
	}
}

void AARPGCharacter::ArmEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AARPGCharacter::SetCharacterState()
{
	if (EquippedWeapon)
	{
		switch (EquippedWeapon->GetWeaponType())
        {
        case EWeaponType::EWT_1HSword:
        	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
        	break;
        case EWeaponType::EWT_2HSword:
        	CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
        	break;
        default:
        	CharacterState = ECharacterState::ECS_Unequipped;
        	break;
        }
	}
	else
	{
		CharacterState = ECharacterState::ECS_Unequipped;
	}
	
}
