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
#include "CustomComponents/AttributesComponent.h"
#include "Widgets/MainOverlay.h"
#include "Widgets/ST_HUD.h"

AARPGCharacter::AARPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
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
	
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		InitializeInputSubsystem();
		InitializeMainOverlay();
	}
	Tags.Add(FName("EngageableTarget"));
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

void AARPGCharacter::Attack()
{
	if (CanAttack())
	{
		/*FName GripName = EquippedWeapon->GetGripName();
		
		FName SectionName = GetRandomSectionByName(AttackMontageToPlay, GripName);
		PlayMontage(AttackMontageToPlay, SectionName);*/
		ActionState = EActionState::EAS_Attacking;
	}
}

float AARPGCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
                                 AActor* DamageCauser)
{
	if (AttributeComponent)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);
		UpdateHealthBar();
	}
	return DamageAmount;
}

void AARPGCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);
	if (AttributeComponent && AttributeComponent->IsAlive())
	{
		ActionState = EActionState::EAS_HitReaction;
	}
	SetEnableBoxCollision(ECollisionEnabled::NoCollision);
}

void AARPGCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState == EActionState::EAS_HitReaction) return;
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Find out forward and right vector based on controller rotation
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	// Forward direction (X axis)
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// Right direction (Y axis)
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Forward / Backward
	if (EquipState == EEquipState::EES_Equipped && EquippedWeapon->GetGripType() == EGripType::EGT_2Hand)
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
	if (OverlappingWeapon && ArmedState == EArmedState::EA_Unarmed)
	{
		SetOverlappingItem(nullptr);
		EquippedWeapon = OverlappingWeapon;
		Arm();
		EquipState = EEquipState::EES_Equipped;
	}
	else
	{
		if (CanDisarm())
		{
			PlayMontage(ArmDisarmMontage, FName("Disarm"));
			EquipState = EEquipState::EES_Equipped;
			ActionState = EActionState::EAS_Arming;
			ArmedState = EArmedState::EA_Unarmed;
		}
		else if (CanArm())
		{
			PlayMontage(ArmDisarmMontage, FName("Arm"));
			EquipState = EEquipState::EES_Equipped;
			ActionState = EActionState::EAS_Arming;
			ArmedState = EArmedState::EA_Armed;
		}
	}
}

bool AARPGCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& EquipState != EEquipState::EES_Unequipped
		&& IsArmed();
}

void AARPGCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AARPGCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& EquipState != EEquipState::EES_Unequipped
		&& ArmedState == EArmedState::EA_Armed;
}

bool AARPGCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		EquipState == EEquipState::EES_Equipped &&
		ArmedState == EArmedState::EA_Unarmed &&
		EquippedWeapon;
}

void AARPGCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		FName SocketName = EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Spear ? FName("SpineSpearSocket") : FName("SpineWeaponSocket");
		EquippedWeapon->Equip(GetMesh(), SocketName, this, this);
		ArmedState = EArmedState::EA_Unarmed;
	}
}

void AARPGCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Equip(GetMesh(), FName("HandGrip_R"), this, this);
		PickAttackMontageByWeaponType();
		ArmedState = EArmedState::EA_Armed;
	}
}

void AARPGCharacter::ArmEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AARPGCharacter::HitReactEnd()
{
	Super::HitReactEnd();
	if (ActionState == EActionState::EAS_HitReaction)
	{
		ActionState = EActionState::EAS_Unoccupied;
	}
}

void AARPGCharacter::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
}

void AARPGCharacter::InitializeInputSubsystem()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(InputActionContext, 0);
	}
}

void AARPGCharacter::InitializeMainOverlay()
{
	AST_HUD* ST_HUD = Cast<AST_HUD>(PlayerController->GetHUD());
	if (ST_HUD)
	{
		MainOverlay = ST_HUD->GetMainOverlay();
		if (MainOverlay)
		{
			MainOverlay->SetHealthPercent(AttributeComponent->GetHealthPercent());
			MainOverlay->SetStaminaPercent(AttributeComponent->GetStaminaPercent());
		}
	}
}

void AARPGCharacter::PickAttackMontageByWeaponType()
{
	switch (EquippedWeapon->GetWeaponType())
	{
	case EWeaponType::EWT_Sword:
		AttackMontageToPlay = SwordAttackMontage;
		break;
	case EWeaponType::EWT_Axe:
		AttackMontageToPlay = SwordAttackMontage;
		break;
	case EWeaponType::EWT_Spear:
		AttackMontageToPlay = SpearAttackMontage;
		break;
	default:
		AttackMontageToPlay = SwordAttackMontage;
		break;
	}
}

FName AARPGCharacter::GetRandomSectionByName(const UAnimMontage* Montage, const FName Name)
{
	if (!Montage || Name == "")
	{
		return FName("None");
	}
	// Collect all sections with names starting with the SectionName parameter
	TArray<FName> MatchingSections;
	for (int32 i = 0; i < AttackMontageToPlay->GetNumSections(); ++i)
	{
		const FName CurrentSectionName = Montage->GetSectionName(i);
		if (CurrentSectionName.ToString().StartsWith(Name.ToString()))
		{
			MatchingSections.Add(CurrentSectionName);
		}
	}
		
	// Randomly select one of the matching sections
	if (MatchingSections.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, MatchingSections.Num() - 1);
		return MatchingSections[RandomIndex];
	}
	return FName("None");
}


void AARPGCharacter::UpdateHealthBar()
{
	if (MainOverlay)
	{
		MainOverlay->SetHealthPercent(AttributeComponent->GetHealthPercent());
	}
}
