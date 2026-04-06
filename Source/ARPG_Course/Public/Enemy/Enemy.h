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
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	/*
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	/*
	 * SOUNDS
	 */
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitMoanSound;

	/*
	 * EFFECTS
	 */
	UPROPERTY(EditAnywhere, Category = "Cascade Effects")
	UParticleSystem* HitEffect;

	/*
	 * COMPONENTS
	 */
	UPROPERTY(VisibleAnywhere)
	UAttributesComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	/*
	 * COMBAT
	 */
	UPROPERTY(VisibleAnywhere)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;
	
	FName CalculateHitDirection(const FVector& ImpactPoint);
	
	void ConfigureCollisionResponces();
//Getters and Setters
public:
	

};
