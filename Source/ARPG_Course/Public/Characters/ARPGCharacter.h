// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "GenericTeamAgentInterface.h"
#include "ARPGCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;


UCLASS()
class ARPG_COURSE_API AARPGCharacter : public ABaseCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AARPGCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Attack() override;
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

	/*
	 * PlayMontages functions
	 */
	virtual void PlayMontage(UAnimMontage* AnimMontageToPlay, const FName& SectionName = "") override;
	
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

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	void PickAttackMontageByWeaponType();
	FName GetRandomSectionByName(const UAnimMontage* Montage, FName Name);
	
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override {if (TeamId != NewTeamId){TeamId = NewTeamId;}};
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	
};
