// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "UpdateMovementSpeedTask.generated.h"

class AEnemy;
class AAIController;
/**
 * StateTree task that changes Enemy's MaxWalkSpeed.
 */
UCLASS(Blueprintable, BlueprintType, meta = (DisplayName = "Update Movement Speed"))
class ARPG_COURSE_API UUpdateMovementSpeedTask : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	/*
	 * Properties
	 */
	/** The Enemy character, whose speed will be updated */
	UPROPERTY(BlueprintReadOnly, Category = "Context", meta = (Context))
	TObjectPtr<AEnemy> Actor;

	/** The Enemy AIController */
	UPROPERTY(BlueprintReadOnly, Category = "Context", meta = (Context))
	TObjectPtr<AAIController> AIController;

	/** Desired max speed to apply */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0"))
	float TargetMaxWalkSpeed;

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
};
