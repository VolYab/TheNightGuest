// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/BaseCharacter.h"
#include "GameplayTagContainer.h"
#include "Enemy.generated.h"

class UItemDataAsset;
class UWidgetComponent;
class UHealthBarComponent;

/**
 * Enumeration representing the action state of an enemy.
 * Used to track the current action being performed by the enemy.
 */
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_HitReaction UMETA(DisplayName = "Hit Reaction"),
	EES_Dead UMETA(DisplayName = "Dead")
};

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
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual void Destroyed() override;

	/**
	 * This function calculates damage on Health of an actor, updates health bar
	 * @param DamageAmount Amount of damage caused.
	 * @param DamageEvent 
	 * @param EventInstigator Controller, which controls damage causer
	 * @param DamageCauser Actor, which provide damage (weapon, for example)
	 * @return 
	 */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Attack() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat")
	AActor* CombatTarget;
	
	/*
	 * Functions
	 */
	virtual void BeginPlay() override;
	
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

	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HitReactEnd() override;

private:
	EEnemyState EnemyState = EEnemyState::EES_Unoccupied;
	
	/*
	 * COMPONENTS
	 */
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	/*
	 * COMBAT
	 */
	UPROPERTY(EditAnywhere, Category="Combat")
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category="Combat")
	double AttackRadius = 150.f;

	UPROPERTY(VisibleAnywhere, Category="Combat")
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, Category="Combat")
	UItemDataAsset* DefaultWeaponData;
	/**
	 * This function configures Collision settings for an Enemy actor
	 */
	void ConfigureCollisionResponces();

	void ShowHealthBar();
	void HideHealthBar();

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

	EEnemyState GetEnemyState() const
	{
		return EnemyState;
	}

	void SetEnemyState(EEnemyState NewEnemyState)
	{
		this->EnemyState = NewEnemyState;
	}
};
