// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class UAttributesComponent;
class AWeapon;
/**
 * A Base class for characters in a game
 */
UCLASS()
class ARPG_COURSE_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	/**
	 * This function enables or disables box collision for an equipped weapon during attack animation 
	 * @param BoxCollisionEnabled CollisionEnabled type to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetEnableBoxCollision(ECollisionEnabled::Type BoxCollisionEnabled);

	/**
	 * This function plays anim reaction montage based on the direction of impact.
	 * @param ImpactPoint Point of impact.
	 */
	void DirectionalHitReact(const FVector& ImpactPoint);

	/**
	 * This function adds animation reaction for hit, shows health bar, play sounds, spawn emitters.
	 * @param ImpactPoint Point of impact.
	 */
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	void SetCharacterState();
protected:
	/**
	 * Properties
	 */
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(BlueprintReadOnly)
	EDeadPose DeadPose = EDeadPose::EAS_Alive;
	
	UPROPERTY(VisibleAnywhere, Category="Combat|Weapon")
	AWeapon* EquippedWeapon;

	/*
	 * COMPONENTS
	 */
	UPROPERTY(VisibleAnywhere)
	UAttributesComponent* AttributeComponent;
	
	/**
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Montages")
	UAnimMontage* SwordAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation|Montages")
	UAnimMontage* SpearAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation|Montages")
	UAnimMontage* ArmDisarmMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation|Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation|Montages")
	UAnimMontage* DeathMontage;

	/*
	 * SOUNDS
	 */
	UPROPERTY(EditAnywhere, Category = "Sounds|Hit")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Sounds|Hit")
	USoundBase* HitMoanSound;

	/*
	 * EFFECTS
	 */
	UPROPERTY(EditAnywhere, Category = "Cascade Effects|Hit")
	UParticleSystem* HitEffect;
	
	/**
	 * Functions
	 */
	virtual void BeginPlay() override;

	/**
	 * This is a base function to perform an attack
	 */
	virtual void Attack();

	/**
	 * This is a function to check if a character can attack
	 * @return true if a character can attack, false if not
	 */
	bool CanAttack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	/**
	 * This function handles death processes
	 */
	virtual void Die();
	
	/*
	 * PlayMontages functions
	 */
	/**
	 * This function plays animation montage
	 * @param AnimMontageToPlay Montage to play.
	 * @param SectionName Section name in montage to play, if provided. If not provided - a random section will be played.
	 */
	virtual void PlayMontage(UAnimMontage* AnimMontageToPlay, const FName& SectionName = "");

private:

	/**
	 * This function returns the direction name from which the hit was struck
	 * @param ImpactPoint Point of hit impact
	 * @return FName of the direction, it may be "FromBack", "FromFront", "FromLeft", "FromRight"
	 */
	FName CalculateHitDirection(const FVector& ImpactPoint);
};
