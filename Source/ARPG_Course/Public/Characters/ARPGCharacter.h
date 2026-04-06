// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "GenericTeamAgentInterface.h"
#include "ARPGCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class ARPG_COURSE_API AARPGCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AARPGCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetEnableBoxCollision(ECollisionEnabled::Type BoxCollisionEnabled);

	virtual void PossessedBy(AController* NewController) override;
protected:
	virtual void BeginPlay() override;

	/*
	 * Input Actions
	 */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputActionContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* EKeyPressedAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	/*
	 * Input callback functions
	 */
	void Move(const FInputActionValue& Value);
	void Lookout(const FInputActionValue& Value);
	void EKeyPressed();
	void Attack();

	/*
	 * PlayMontages functions
	 */
	void PlayMontage(UAnimMontage* AnimMontageToPlay, const FName& SectionName = "");
	//void PlayAttackMontage();
	//void PlayArmDisarmMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void ArmEnd();
private:
	UPROPERTY(VisibleAnywhere)
	FGenericTeamId TeamId;
	
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	AWeapon* EquippedWeapon;

	// Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* SwordAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* SpearAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* ArmDisarmMontage;

public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override {if (TeamId != NewTeamId){TeamId = NewTeamId;}};
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	void SetCharacterState();
};
