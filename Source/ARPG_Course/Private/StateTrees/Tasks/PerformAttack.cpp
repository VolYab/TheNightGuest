// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTrees/Tasks/PerformAttack.h"

#include "Enemy/Enemy.h"

EStateTreeRunStatus UPerformAttack::EnterState(FStateTreeExecutionContext& Context,
                                               const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	if (!Actor)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	Actor->PerformAttack();
	return EStateTreeRunStatus::Succeeded;
}
