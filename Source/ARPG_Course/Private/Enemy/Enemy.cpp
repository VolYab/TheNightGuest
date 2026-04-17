// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "CustomComponents/AttributesComponent.h"
#include "Widgets/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::Attack()
{
	/*Super::Attack();*/
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if target is not in chasing (combat) radius - reset CombatTarget, send Enemy patrolling
	if (!TargetInRange(CombatTarget, CombatRadius))
	{
		CombatTarget = nullptr;
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
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

void AEnemy::PerformAttack()
{
	Attack();
}

void AEnemy::PlayMontage(UAnimMontage* AnimMontageToPlay, const FName& SectionName)
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
			//Check the number of sections in AnimMontage to generate a random index
			const int32 NumberOfSections = AnimMontageToPlay->GetNumSections() - 1;
			const int32 RandomSectionIndex = FMath::RandRange(0, NumberOfSections);
			//Get Section Name using a random index
			const FName RandomSectionName = AnimMontageToPlay->GetSectionName(RandomSectionIndex);
			AnimInstance->Montage_JumpToSection(RandomSectionName, AnimMontageToPlay);
		}
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

void AEnemy::Die()
{
	Super::Die();
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

bool AEnemy::TargetInRange(AActor* Target, float RangeRadius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= RangeRadius;	
}
