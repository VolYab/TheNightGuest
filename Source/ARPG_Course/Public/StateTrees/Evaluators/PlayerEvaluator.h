// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "PlayerEvaluator.generated.h"

class AAIController;
class AEnemy;
/**
 * Evaluator that monitors the player
*/
UCLASS()
class ARPG_COURSE_API UPlayerEvaluator : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()

public:
	virtual void TreeStart(FStateTreeExecutionContext& Context) override;
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	virtual void TreeStop(FStateTreeExecutionContext& Context) override;

	/* The Enemy character, owning the StateTree */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta = (AllowPrivateAccess = "true"))
	APawn* Character;

	/* Enemy AI Controller */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Context, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AAIController> AIController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Output, meta = (AllowPrivateAccess = "true"))
	float DistanceToPlayer = 0.f;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Output)
	bool IsPlayerDead = false;

private:
	void SetDefaultValues();
};
