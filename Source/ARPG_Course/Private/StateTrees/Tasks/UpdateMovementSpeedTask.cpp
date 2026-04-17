// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTrees/Tasks/UpdateMovementSpeedTask.h"

#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

EStateTreeRunStatus UUpdateMovementSpeedTask::EnterState(FStateTreeExecutionContext& Context,
                                                         const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	if (!Actor)
	{
		return EStateTreeRunStatus::Failed;
	}
	if (UCharacterMovementComponent* MovementComponent = Actor->GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = TargetMaxWalkSpeed;
		return EStateTreeRunStatus::Succeeded;
	}
	// If reached here - fail the task
	return EStateTreeRunStatus::Failed;
}
