// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "PerformAttack.generated.h"

class AAIController;
class AEnemy;
/**
 * StateTree task that performs Enemy's attack.
 */
UCLASS(Blueprintable, BlueprintType, meta = (DisplayName = "Perform attack"))
class ARPG_COURSE_API UPerformAttack : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

	// Constructor
	UPerformAttack(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		bShouldCallTick = true; // Enables ticking every frame
	}
public:
	/** The Enemy character, who performs an attack */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Context", meta = (Context))
	TObjectPtr<AEnemy> Actor;

	/** The Enemy AIController */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Context", meta = (Context))
	TObjectPtr<AAIController> AIController;

protected:
	/*
	 * Functions
	*/

	/**
	 * This function sets the character's MaxWalkSpeed to the TargetMaxWalkSpeed when entering the state.
	 * @param Context The execution context for the state tree.
	 * @param Transition The transition result containing information about the state change.
	*/
	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	/**
	 * Called when state ticks
	 * @param Context The execution context for the state tree
	 * @param DeltaTime 
	 */
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	/**
	 * Called when state finishes
	 * @param Context The execution context for the state tree
	 * @param Transition The transition that triggered this state
	 */
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
private:
	/*
	 * Properties
	*/

	// Indicates whether the montage has finished playing
	bool bIsAttackMontageFinished = false;
};
