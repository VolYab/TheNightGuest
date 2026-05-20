// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "CustomComponents/AttributesComponent.h"
#include "DataAssets/ItemDataAsset.h"
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
	HideHealthBar();

	AWeapon* DefaultWeapon = Cast<AWeapon>(AItemSpawner::SpawnItem(this, DefaultWeaponData));
	DefaultWeapon->Equip(GetMesh(), FName("HandGrip_R"), this, this);
	EquippedWeapon = DefaultWeapon;
}



void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!TargetInRange(CombatTarget, CombatRadius))
	{
		CombatTarget = nullptr;
		HideHealthBar();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	ShowHealthBar();
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
	
	if (SwordAttackMontage)
	{
		PlayMontage(SwordAttackMontage);
	}
	EnemyState = EEnemyState::EES_Attacking;
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
	HideHealthBar();
	Super::Die();
}

bool AEnemy::TargetInRange(AActor* Target, float RangeRadius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= RangeRadius;	
}
