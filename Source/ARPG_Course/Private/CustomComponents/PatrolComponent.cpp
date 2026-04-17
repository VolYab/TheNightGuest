// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomComponents/PatrolComponent.h"
#include "ARPG_Course/DebugMacros.h"
#include "AIModule.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

UPatrolComponent::UPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}

void UPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	AIEnemyController = Cast<AAIController>(Cast<APawn>(GetOwner())->GetController());
	if (AIEnemyController)
	{
		if (!CurrentPatrolTarget)
		{
			SetCurrentPatrolTarget(GetRandomPatrolTarget());
		}
		GoToTarget(CurrentPatrolTarget);
	}
}

void UPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//PatrolPoints();
}

void UPatrolComponent::PatrolPoints()
{
    //Check if the patrolling actor reached CurrentPatrolTarget
    if (PatrolPointReached(CurrentPatrolTarget, PatrolRadius))
    {
    	UE_LOG(LogTemp, Warning, TEXT("PatrolPointReached"));
    	SetCurrentPatrolTarget(GetRandomPatrolTarget());
    	GetOwner()->GetWorldTimerManager().SetTimer(PatrolTimer, this, &UPatrolComponent::PatrolTimerFinished, 5.f);
    }
}

void UPatrolComponent::GoToTarget(AActor* Target)
{
	if (AIEnemyController && Target)
	{
		FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalActor(Target);
        MoveRequest.SetAcceptanceRadius(15.f);
		AIEnemyController->MoveTo(MoveRequest);
	}
}

void UPatrolComponent::GoToRandomPoint()
{
	if (AIEnemyController)
	{
		SetCurrentPatrolTarget(GetRandomPatrolTarget());
		GoToTarget(CurrentPatrolTarget);
	}
}

bool UPatrolComponent::PatrolPointReached(AActor* PatrolPoint, float RangeRadius) const
{
	DRAW_SPHERE_SingleFrame(PatrolPoint->GetActorLocation());
	const double DistanceToTarget = (PatrolPoint->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
	return DistanceToTarget <= RangeRadius;
}

AActor* UPatrolComponent::GetRandomPatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != CurrentPatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	if (ValidTargets.Num() != 0)
	{
		int32 TargetNumber = FMath::RandRange(0, ValidTargets.Num() - 1);
		return ValidTargets[TargetNumber];
	}
	return nullptr;
}

void UPatrolComponent::PatrolTimerFinished()
{
	GoToTarget(CurrentPatrolTarget);
}

void UPatrolComponent::ClearPatrolTimer()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(PatrolTimer);
}

class AAIController* UPatrolComponent::GetAIEnemyController() const
{
	return AIEnemyController;
}

void UPatrolComponent::SetAIEnemyController(class AAIController* NewAIEnemyController)
{
	this->AIEnemyController = NewAIEnemyController;
}

AActor* UPatrolComponent::GetCurrentPatrolTarget() const
{
	return CurrentPatrolTarget;
}

void UPatrolComponent::SetCurrentPatrolTarget(AActor* NewCurrentPatrolTarget)
{
	this->CurrentPatrolTarget = NewCurrentPatrolTarget;
}

TArray<AActor*> UPatrolComponent::GetPatrolTargets() const
{
	return PatrolTargets;
}

void UPatrolComponent::SetPatrolTargets(const TArray<AActor*>& NewPatrolTargets)
{
	this->PatrolTargets = NewPatrolTargets;
}

double UPatrolComponent::GetPatrolRadius() const
{
	return PatrolRadius;
}

void UPatrolComponent::SetPatrolRadius(double NewPatrolRadius)
{
	this->PatrolRadius = NewPatrolRadius;
}
