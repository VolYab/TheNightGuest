// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTrees/Tasks/PerformAttack.h"

#include "Enemy/Enemy.h"

EStateTreeRunStatus UPerformAttack::EnterState(FStateTreeExecutionContext& Context,
                                               const FStateTreeTransitionResult& Transition)
{
	if (!Actor)
	{
		return EStateTreeRunStatus::Failed;
	}
	Enemy = Cast<AEnemy>(Actor);
	if (!Enemy)
	{
		return EStateTreeRunStatus::Failed;
	}
	Enemy->Attack();
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus UPerformAttack::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (Enemy && Enemy->GetEnemyState() == EEnemyState::EES_Unoccupied)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	return EStateTreeRunStatus::Running;
}

void UPerformAttack::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
}
