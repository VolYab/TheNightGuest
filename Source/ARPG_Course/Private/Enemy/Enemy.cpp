// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "ARPG_Course/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ConfigureCollisionResponces();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	DirectionalHitReact(ImpactPoint);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, ImpactPoint);
	}
	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, ImpactPoint, FRotator(90.f, ImpactPoint.Y, ImpactPoint.Z));
	}
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
			//Check number of sections in AnimMontage to generate random index
			const int32 NumberOfSections = AnimMontageToPlay->GetNumSections() - 1;
			const int32 RandomSectionIndex = FMath::RandRange(0, NumberOfSections);
			//Get Section Name using random index
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

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	/*
	 * To play montage based on hit direction, calculate angle between Forward vector and HitImpact Vector
	 * To calculate angle between Forward vector and HitImpact Vector, DotProduct is used
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

	//if cross product vector points down - theta should be negative
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
	
	PlayMontage(HitReactMontage, AnimSectionName);
}
