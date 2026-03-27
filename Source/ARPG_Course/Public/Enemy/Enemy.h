// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributesComponent;
class UWidgetComponent;
class UHealthBarComponent;
class UPatrolComponent;

UCLASS()
class ARPG_COURSE_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	UPROPERTY(BlueprintReadOnly)
	EDeadPose DeadPose = EDeadPose::EAS_Alive;
	
	virtual void BeginPlay() override;

	void Die();
	bool TargetInRange(AActor* Target, float RangeRadius);
	/*
	 * PlayMontages functions
	 */
	void PlayMontage(UAnimMontage* AnimMontageToPlay, const FName& SectionName = "");

private:
	/* START VARIABLES */
	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitMoanSound;

	UPROPERTY(EditAnywhere, Category = "Cascade Effects")
	UParticleSystem* HitEffect;

	UPROPERTY(VisibleAnywhere)
	UAttributesComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UPatrolComponent* PatrolComponent;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;
	
	/* END VARIABLES */

	/* START FUNCTIONS */
	void ConfigureCollisionResponces();
	FName CalculateHitDirection(const FVector& ImpactPoint);
	/* END FUNCTIONS */
//Getters and Setters
public:
	

};
