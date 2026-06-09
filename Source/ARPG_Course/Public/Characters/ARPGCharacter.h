// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "GenericTeamAgentInterface.h"
#include "ARPGCharacter.generated.h"

class UMainOverlay;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;

/**
 * Enumeration representing the action state of a character.
 * Used to track the current action being performed by the character.
 */
UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "Hit Reaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Arming UMETA(DisplayName = "Arming"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
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

	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void ArmEnd();
	
	virtual void HitReactEnd() override;
	virtual void Die();
private:
	UPROPERTY()
	APlayerController* PlayerController;
	
	UPROPERTY(VisibleAnywhere)
	FGenericTeamId TeamId;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY()
	UMainOverlay* MainOverlay;

	/**
	 * This function sets up the Input system for the player character
	 */
	void InitializeInputSubsystem();

	/**
	 * This function sets up the Main overlay widgets
	 */
	void InitializeMainOverlay();

	/**
	 * This function checks the current EquippedWeapon type and sets AttackMontageToPlay to correct montage
	 */
	void PickAttackMontageByWeaponType();

	/**
	 * This function randomly selects one of the sections with names starting with the SectionName parameter
	 * @param Montage AnimMontage to pick section from
	 * @param Name Name to filter sections
	 * @return 
	 */
	FName GetRandomSectionByName(const UAnimMontage* Montage, FName Name);

	void UpdateHealthBar();
	
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override {if (TeamId != NewTeamId){TeamId = NewTeamId;}};
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	
};
