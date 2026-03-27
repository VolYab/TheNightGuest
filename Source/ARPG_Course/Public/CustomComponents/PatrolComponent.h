// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARPG_COURSE_API UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPatrolComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void PatrolPoints();
	
protected:
	virtual void BeginPlay() override;
	void GoToTarget(AActor* Target);
    void GoToRandomPoint();
    bool PatrolPointReached(AActor* PatrolPoint, float RangeRadius) const;
    AActor* GetRandomPatrolTarget();
private:
	/*Navigation*/
	UPROPERTY()
	class AAIController* AIEnemyController;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	AActor* CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category="AI Navigation")
	double PatrolRadius = 100.f;

// GETTERS AND SETTERS
public:
	class AAIController* GetAIEnemyController() const;
	void SetAIEnemyController(class AAIController* NewAIEnemyController);
	AActor* GetCurrentPatrolTarget() const;
	void SetCurrentPatrolTarget(AActor* NewCurrentPatrolTarget);
	TArray<AActor*> GetPatrolTargets() const;
	void SetPatrolTargets(const TArray<AActor*>& NewPatrolTargets);
	double GetPatrolRadius() const;
	void SetPatrolRadius(double NewPatrolRadius);
	
};
