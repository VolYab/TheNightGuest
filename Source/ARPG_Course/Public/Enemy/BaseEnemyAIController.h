// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseEnemyAIController.generated.h"

class UStateTreeAIComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;
class AActor;
struct FAIStimulus;

/**
 * Enemy AI Controller that owns Perception and StateTree components.
 */
UCLASS()
class ARPG_COURSE_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseEnemyAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Configures the parameters of the sight sense for an AI perception system, including range, angle, and detection settings.
	 */
	void ConfigureSightSense();
	/**
	 * Configures the parameters of the damage sense for an AI perception system.
	 * This involves setting up the default damage sense configuration object.
	 */
	void ConfigureDamageSense();
	/**
	 * Sets up callbacks for AI perception events, enabling interaction with perception updates, target perception updates,
	 * and forgotten perceptions. Configures the dominant sense as sight and binds the appropriate handlers.
	 */
	void ConfigurePerceptionCallbacks();
	/**
	 * Configures and initializes the AI Perception system for the controller.
	 */
	void SetupAIPerception();
	
	//~ Begin IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	//~ End IGenericTeamAgentInterface

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> PerceivedEnemy;
		
	// AI Perception component responsible for sensing.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta=(AllowPrivateAccess="true"))
	UAIPerceptionComponent* AIPerception;

	// Gameplay StateTree component for high-level AI logic.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess="true"))
	UStateTreeAIComponent* StateTreeComponent;

	// Perception sense configurations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception", meta=(AllowPrivateAccess="true"))
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception", meta=(AllowPrivateAccess="true"))
	UAISenseConfig_Damage* DamageConfig;

	// Perception callbacks
	UFUNCTION()
	void HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void HandleTargetPerceptionForgotten(AActor* Actor);

	virtual void BeginPlay() override;
private:
	// Team identity used by perception/affiliation checks.
	// Differs from AARPGPlayerController (team 2) so enemies treat the player as hostile.
	FGenericTeamId TeamId;

	void SendStateTreeEvent(FName EventName);
};
