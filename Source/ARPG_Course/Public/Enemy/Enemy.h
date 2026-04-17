// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UWidgetComponent;
class UHealthBarComponent;

/** Base class for enemies */
UCLASS()
class ARPG_COURSE_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	/*
	 * Functions
	*/
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	/**
	 * This function calculates damage on Health of an actor, updates health bar
	 * @param DamageAmount Amount of damage caused.
	 * @param DamageEvent 
	 * @param EventInstigator Controller, which controls damage causer
	 * @param DamageCauser Actor, which provide damage (weapon, for example)
	 * @return 
	 */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/**
	 * This is general a function to perform attack using an Equipped weapon 
	 */
	void PerformAttack();
protected:
	/*
	 * Properties
	 */
	

	/*
	 * Functions
	 */
	virtual void BeginPlay() override;

	virtual void Attack() override;
	/**
	 * This function handles death processes
	 */
	virtual void Die() override;

	/**
	 * This function checks if Target is located within range from the current Actor 
	 * @param Target Actor to check is within range.
	 * @param RangeRadius
	 * @return True if Target is within RangeRadius, false if is not.
	 */
	bool TargetInRange(AActor* Target, float RangeRadius);

	/*
	 * PlayMontages functions
	 */
	/**
	 * This function plays animation montage
	 * @param AnimMontageToPlay Montage to play.
	 * @param SectionName Section name in montage to play, if provided. If not provided - a random section will be played.
	 */
	virtual void PlayMontage(UAnimMontage* AnimMontageToPlay, const FName& SectionName = "") override;

private:
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	/*
	 * COMPONENTS
	 */
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	/*
	 * COMBAT
	 */
	UPROPERTY(VisibleAnywhere, Category="Combat")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category="Combat")
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category="Combat")
	double AttackRadius = 150.f;

	UPROPERTY(VisibleAnywhere, Category="Combat")
	bool bIsAttacking = false;

	/**
	 * This function configures Collision settings for an Enemy actor
	 */
	void ConfigureCollisionResponces();
/**
 * Getters and Setters
 */
public:
	bool IsIsAttacking() const
	{
		return bIsAttacking;
	}

	void SetIsAttacking(bool NewBIsAttacking)
	{
		this->bIsAttacking = NewBIsAttacking;
	}
};
