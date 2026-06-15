// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTrees/Evaluators/PlayerEvaluator.h"

#include "Characters/ARPGCharacter.h"
#include "Enemy/Enemy.h"


void UPlayerEvaluator::TreeStart(FStateTreeExecutionContext& Context)
{
	
}

void UPlayerEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	UWorld* World = Character->GetWorld();
	if (!World)
	{
		SetDefaultValues();
		return;
	}
	APawn* PlayerPawn = World->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn)
	{
		SetDefaultValues();
		return;
	}
	DistanceToPlayer = FVector::Dist(Character->GetActorLocation(), PlayerPawn->GetActorLocation());
	AARPGCharacter* PlayerCharacter = Cast<AARPGCharacter>(PlayerPawn);
	if (PlayerCharacter)
	{
		IsPlayerDead = !PlayerCharacter->IsAlive();
	}
}

void UPlayerEvaluator::TreeStop(FStateTreeExecutionContext& Context)
{
	SetDefaultValues();
}

void UPlayerEvaluator::SetDefaultValues()
{
	DistanceToPlayer = 0.f;
	IsPlayerDead = false;
}
