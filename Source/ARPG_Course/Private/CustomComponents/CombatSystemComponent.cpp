// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomComponents/CombatSystemComponent.h"
#include "Chooser.h"
#include "ChooserFunctionLibrary.h"

UCombatSystemComponent::UCombatSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatSystemComponent::IncreaseComboCount()
{
	ComboCount++;
}

void UCombatSystemComponent::ResetComboCount()
{
	ComboCount = 0;
}

void UCombatSystemComponent::ResetComboCD()
{
	
}

void UCombatSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatSystemComponent::StartComboAttack()
{
	
}

UAnimMontage* UCombatSystemComponent::SelectAssetFromChooser()
{
	if (!WeaponChooserTable) return nullptr;

	FChooserEvaluationContext Context;
	Context.AddObjectParam(StaticClass());
	
	TArray<UObject*> SelectedResults = UChooserFunctionLibrary::EvaluateChooserMulti(
		this, 
		WeaponChooserTable, 
		UAnimMontage::StaticClass()
	);

	// Process the matched assets
	if (SelectedResults.Num() > 0)
	{
		return  Cast<UAnimMontage>(SelectedResults[0]);
	}
	return nullptr;
}
