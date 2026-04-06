// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "CustomComponents/AttributesComponent.h"
#include "Widgets/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ConfigureCollisionResponces();

	AttributeComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("AttributesComponent"));
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
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
	DirectionalHitReact(ImpactPoint);
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, ImpactPoint);
	}
	if (HitMoanSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitMoanSound, ImpactPoint);
	}
	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, ImpactPoint, FRotator(90.f, ImpactPoint.Y, ImpactPoint.Z));
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

FName AEnemy::CalculateHitDirection(const FVector& ImpactPoint)
{
	/*
	 * To play montage based on a hit direction, calculate an angle between Forward vector and HitImpact Vector
	 * To calculate an angle between Forward vector and HitImpact Vector, DotProduct is used
	 * Formula is - DotProduct = |a|*|b|*cos(θ)
	 * As vectors are normalised - so |a|=0 and |b|=0
	 * So cos(θ) = DotProduct
	 * θ = acos(DotProduct)
	 */
	const FVector ImpactLowered = FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHitVector = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double DotProduct = FVector::DotProduct(ToHitVector, GetActorForwardVector());
	//convert radians to degrees
	double Theta = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

	//if the cross-product vector points down - theta should be negative
	FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), ToHitVector);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1;
	}

	FName AnimSectionName = FName("FromBack");
	if(Theta >= -45.0f && Theta < 45.0f)
	{
		AnimSectionName = FName("FromFront");
	} else if (Theta >= 45.0f && Theta < 135.0f)
	{
		AnimSectionName = FName("FromRight");
	} else if (Theta >= -135.0f && Theta < -45.0f)
	{
		AnimSectionName = FName("FromLeft");
	}
	return AnimSectionName;
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	FName AnimSectionName = CalculateHitDirection(ImpactPoint);
	if (AttributeComponent && AttributeComponent->IsAlive())
	{
		PlayMontage(HitReactMontage, AnimSectionName);
	}
	else
	{
		if(AnimSectionName == "FromFront")
		{
			DeadPose = EDeadPose::EAS_DeadBackward;
		} else if (AnimSectionName == "FromRight")
		{
			DeadPose = EDeadPose::EAS_DeadLeft;
		} else if (AnimSectionName == "FromLeft")
		{
			DeadPose = EDeadPose::EAS_DeadRight;
		} else
		{
			DeadPose = EDeadPose::EAS_DeadForward;
		}
		Die();
	}
}

void AEnemy::Die()
{
	FName AnimSectionName;
	switch (DeadPose)
	{
	case EDeadPose::EAS_DeadForward:
		AnimSectionName = FName("FromBack");
		break;
	case EDeadPose::EAS_DeadRight:
		AnimSectionName = FName("FromLeft");
		break;
	case EDeadPose::EAS_DeadLeft:
		AnimSectionName = FName("FromRight");
		break;
	default:
		AnimSectionName = FName("FromFront");
	}
	PlayMontage(DeathMontage, AnimSectionName);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

bool AEnemy::TargetInRange(AActor* Target, float RangeRadius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= RangeRadius;	
}
