// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionTypes.h"

ABaseEnemyAIController::ABaseEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ABaseEnemyAIController::SetGenericTeamId(FGenericTeamId(1));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeComponent"));

	SetupAIPerception();
}

void ABaseEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	StateTreeComponent->StartLogic();
}

void ABaseEnemyAIController::ConfigureSightSense()
{
	// Create and configure Sight sense
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.0f;
		SightConfig->LoseSightRadius = 1800.0f;
		SightConfig->PeripheralVisionAngleDegrees = 60.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	}
}

void ABaseEnemyAIController::ConfigureDamageSense()
{
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
}

void ABaseEnemyAIController::ConfigurePerceptionCallbacks()
{
	if (AIPerception)
	{
		AIPerception->SetDominantSense(UAISense_Sight::StaticClass());
		if (SightConfig)
		{
			AIPerception->ConfigureSense(*SightConfig);
		}
		if (DamageConfig)
		{
			AIPerception->ConfigureSense(*DamageConfig);
		}

		// Bind perception callbacks
		AIPerception->OnPerceptionUpdated.AddDynamic(this, &ABaseEnemyAIController::HandlePerceptionUpdated);
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseEnemyAIController::HandleTargetPerceptionUpdated);
		AIPerception->OnTargetPerceptionForgotten.AddDynamic(this, &ABaseEnemyAIController::HandleTargetPerceptionForgotten);
	}
}

void ABaseEnemyAIController::SetupAIPerception()
{
	ConfigureSightSense();
	ConfigureDamageSense();
	ConfigurePerceptionCallbacks();
}

void ABaseEnemyAIController::HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	UE_LOG(LogTemp, Warning, TEXT("Perception updated: %d actor(s)"), UpdatedActors.Num());
}

void ABaseEnemyAIController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		return;
	}

	const bool isEngageableTarget = Actor->ActorHasTag(FName("EngageableTarget"));
	const bool bSensed = Stimulus.WasSuccessfullySensed();
	if (bSensed && isEngageableTarget)
	{
		PerceivedEnemy = Actor;
		// trigger Aggressive state
		SendStateTreeEvent(FName("StateTree.Enemy.Aggressive"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Target perception %s: %s (Tag: %s)"),
		bSensed ? TEXT("updated") : TEXT("lost"),
		*Actor->GetName(),
		*Stimulus.Tag.ToString());
}

void ABaseEnemyAIController::HandleTargetPerceptionForgotten(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Target perception forgotten: %s"), *Actor->GetName());
}

void ABaseEnemyAIController::SendStateTreeEvent(FName EventName)
{
	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(EventName);
	FStateTreeEvent StateTreeEvent;
	StateTreeEvent.Tag = EventTag;
	
	if (!StateTreeComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("StateTreeComponent is null, can't send StateTreeEvent!"));
		return;
	}
	StateTreeComponent->SendStateTreeEvent(StateTreeEvent);
}

void ABaseEnemyAIController::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	if (TeamId != NewTeamId)
	{
		TeamId = NewTeamId;
	}
}
