// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_COURSE_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributesComponent();

virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	//VARIABLES
	UPROPERTY(EditAnywhere, Category = "Actor attributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Actor attributes")
	float MaxHealth;

	//FUNCTIONS
	virtual void BeginPlay() override;

// GETTERS AND SETTERS
public:
	float GetCurrentHealth() const;
	void ReceiveDamage(float DamageValue);
	float GetMaxHealth() const;
	void SetMaxHealth(float Health);
};
