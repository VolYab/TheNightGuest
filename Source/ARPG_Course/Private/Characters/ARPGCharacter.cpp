// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ARPGCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Characters/ARPGPlayerController.h"

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
	Tags.Add(FName("ARPGCharacter"));
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

void AARPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IGenericTeamAgentInterface* ControllerAsTeamProvider = Cast<IGenericTeamAgentInterface>(NewController))
	{
		TeamId = ControllerAsTeamProvider->GetGenericTeamId();
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
	if (CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon || CharacterState == ECharacterState::ECS_EquippedTwoHandedSpear)
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
		OverlappingWeapon->Equip(GetMesh(), FName("HandGrip_R"), this, this);
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
		switch(CharacterState)
		{
		case ECharacterState::ECS_EquippedTwoHandedWeapon:
			PlayMontage(SwordAttackMontage, FName("Attack_2HWeapon"));
			break;
		case ECharacterState::ECS_EquippedOneHandedWeapon:
			PlayMontage(SwordAttackMontage, FName("Attack_1HWeapon"));
			break;
		case ECharacterState::ECS_EquippedOneHandedSpear:
			PlayMontage(SpearAttackMontage, FName("Attack_1HWeapon"));
			break;
		case ECharacterState::ECS_EquippedTwoHandedSpear:
			PlayMontage(SpearAttackMontage, FName("Attack_2HWeapon"));
			break;
		default:
			PlayMontage(SwordAttackMontage);
			break;
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
		if (SectionName == "")
		{
			//Check the number of sections in AnimMontage to generate a random index
			const int32 NumberOfSections = AnimMontageToPlay->GetNumSections() - 1;
			const int32 RandomSectionIndex = FMath::RandRange(0, NumberOfSections);
			//Get Section Name using a random index
			const FName RandomSectionName = AnimMontageToPlay->GetSectionName(RandomSectionIndex);
			AnimInstance->Montage_JumpToSection(RandomSectionName, AnimMontageToPlay);
		}
		else
		{
			// Collect all sections with names starting with the SectionName parameter
			TArray<FName> MatchingSections;
			for (int32 i = 0; i < AnimMontageToPlay->GetNumSections(); ++i)
			{
				const FName CurrentSectionName = AnimMontageToPlay->GetSectionName(i);
				if (CurrentSectionName.ToString().StartsWith(SectionName.ToString()))
				{
					MatchingSections.Add(CurrentSectionName);
				}
			}

			// Randomly select one of the matching sections
			if (MatchingSections.Num() > 0)
			{
				const int32 RandomIndex = FMath::RandRange(0, MatchingSections.Num() - 1);
				AnimInstance->Montage_JumpToSection(MatchingSections[RandomIndex], AnimMontageToPlay);
			}
		}
	}
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
		FName SocketName;
		switch (EquippedWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_1HSpear:
			SocketName = FName("Spine1HSpearSocket");
			break;
		case EWeaponType::EWT_2HSpear:
			SocketName = FName("Spine2HSpearSocket");
			break;
		default:
			SocketName = FName("SpineWeaponSocket");
		}
		EquippedWeapon->Equip(GetMesh(), SocketName, this, this);
	}
}

void AARPGCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Equip(GetMesh(), FName("HandGrip_R"), this, this);
	}
}

void AARPGCharacter::ArmEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
