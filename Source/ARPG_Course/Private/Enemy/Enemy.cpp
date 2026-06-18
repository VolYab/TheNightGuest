// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "CustomComponents/AttributesComponent.h"
#include "Factories/ItemSpawner.h"
#include "Widgets/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ConfigureCollisionResponces();
	
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComponent"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Enemy"));
	HideHealthBar();

	if (AItem* Item = AItemSpawner::SpawnItem(this, DefaultWeaponData))
	{
		AWeapon* DefaultWeapon = Cast<AWeapon>(Item);
		if (IsValid(DefaultWeapon->GetStaticMeshComponent()))
		{
			DefaultWeapon->Equip(GetMesh(), FName("HandGrip_R"), this, this);
            EquippedWeapon = DefaultWeapon;
            EquipState = EEquipState::EES_Equipped;
            ArmedState = EArmedState::EA_Armed;
		}
	}
}



void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*if (!TargetInRange(CombatTarget, CombatRadius))
	{
		CombatTarget = nullptr;
		HideHealthBar();
	}*/
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	ShowHealthBar();
	EnemyState = EEnemyState::EES_HitReaction;
	Super::GetHit_Implementation(ImpactPoint);
}

void AEnemy::Destroyed()
{
	Super::Destroyed();
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
                         AActor* DamageCauser)
{
	if (AttributeComponent)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);
		if (HealthBarWidget)
		{
			HealthBarWidget->SetHealthPercent(AttributeComponent->GetCurrentHealth() / AttributeComponent->GetMaxHealth());
		}
	}
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

void AEnemy::Attack()
{
	//TODO add check if can attack (not dead, not playing hit animation, has weapon)
	if (CanAttack())
	{
		if (SwordAttackMontage)
        {
        	PlayMontage(SwordAttackMontage);
        }
        EnemyState = EEnemyState::EES_Attacking;
		bIsAttacking = true;
	}
}

void AEnemy::ConfigureCollisionResponces()
{
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::Die()
{
	EnemyState = EEnemyState::EES_Dead;
	HideHealthBar();
	Super::Die();
	EquippedWeapon->GetWeaponBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AEnemy::TargetInRange(AActor* Target, float RangeRadius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= RangeRadius;	
}

bool AEnemy::CanAttack()
{
	return EnemyState == EEnemyState::EES_Unoccupied && EquipState != EEquipState::EES_Unequipped;
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_Unoccupied;
}

void AEnemy::HitReactEnd()
{
	EnemyState = EEnemyState::EES_Unoccupied;
}
