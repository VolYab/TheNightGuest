// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomComponents/AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

}

float UAttributesComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

void UAttributesComponent::ReceiveDamage(float DamageValue)
{
	if (CurrentHealth > 0)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - DamageValue, 0.f, MaxHealth);
	}
}

float UAttributesComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UAttributesComponent::SetMaxHealth(float Health)
{
	this->MaxHealth = Health;
}

void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UAttributesComponent::IsAlive() const
{
	return CurrentHealth > 0;
}

