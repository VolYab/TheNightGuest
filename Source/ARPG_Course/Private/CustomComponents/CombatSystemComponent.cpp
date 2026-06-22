// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomComponents/CombatSystemComponent.h"
#include "Chooser.h"
#include "ChooserFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/WeaponEventManager.h"

UCombatSystemComponent::UCombatSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	FChooserEvaluationContext Context;
	Context.AddObjectParam(StaticClass());
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		UWeaponEventManager* WeaponEventManager = GameInstance->GetSubsystem<UWeaponEventManager>();
		if (WeaponEventManager)
		{
			WeaponEventManager->OnWeaponArmed.AddUniqueDynamic(this, &UCombatSystemComponent::SetWeaponProperties);
		}
	}
}

void UCombatSystemComponent::IncreaseComboCount()
{
	ComboCount++;
	ResetComboCD();
}

void UCombatSystemComponent::ResetComboCount()
{
	ComboCount = 0;
	ResetComboCD();
}

void UCombatSystemComponent::ResetComboCD()
{
	CanAttack = true;
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		UWeaponEventManager* WeaponEventManager = GameInstance->GetSubsystem<UWeaponEventManager>();
		if (WeaponEventManager)
		{
			WeaponEventManager->OnComboChange.Broadcast(CanAttack);
		}
	}
}

void UCombatSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatSystemComponent::SetWeaponProperties(EWeaponType NewWeaponType, EGripType NewGripType)
{
	WeaponType = NewWeaponType;
	GripType = NewGripType;
}

UAnimMontage* UCombatSystemComponent::SelectAssetFromChooser()
{
	if (!WeaponChooserTable) return nullptr;
	
	TArray<UObject*> SelectedResults = UChooserFunctionLibrary::EvaluateChooserMulti(
		this, 
		WeaponChooserTable, 
		UAnimMontage::StaticClass()
	);

	// Process the matched assets
	if (SelectedResults.Num() > 0)
	{
		CanAttack = false;
		if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld()))
		{
			UWeaponEventManager* WeaponEventManager = GameInstance->GetSubsystem<UWeaponEventManager>();
			if (WeaponEventManager)
			{
				WeaponEventManager->OnComboChange.Broadcast(CanAttack);
			}
		}
		return Cast<UAnimMontage>(SelectedResults[0]);
	}
	return nullptr;
}
