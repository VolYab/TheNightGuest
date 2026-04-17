// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "CustomComponents/AttributesComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("AttributesComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::Attack()
{
}

bool ABaseCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

void ABaseCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ABaseCharacter::Die()
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
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

void ABaseCharacter::PlayMontage(UAnimMontage* AnimMontageToPlay, const FName& SectionName)
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

FName ABaseCharacter::CalculateHitDirection(const FVector& ImpactPoint)
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

	FName DiretionName = FName("FromBack");
	if(Theta >= -45.0f && Theta < 45.0f)
	{
		DiretionName = FName("FromFront");
	} else if (Theta >= 45.0f && Theta < 135.0f)
	{
		DiretionName = FName("FromRight");
	} else if (Theta >= -135.0f && Theta < -45.0f)
	{
		DiretionName = FName("FromLeft");
	}
	return DiretionName;
}

void ABaseCharacter::SetEnableBoxCollision(ECollisionEnabled::Type BoxCollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBoxComponent())
	{
		EquippedWeapon->GetWeaponBoxComponent()->SetCollisionEnabled(BoxCollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
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

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
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

void ABaseCharacter::SetCharacterState()
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
		case EWeaponType::EWT_1HSpear:
			CharacterState = ECharacterState::ECS_EquippedOneHandedSpear;
			break;
		case EWeaponType::EWT_2HSpear:
			CharacterState = ECharacterState::ECS_EquippedTwoHandedSpear;
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

