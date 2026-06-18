// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomComponents/CombatSystemComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WeaponEventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponArmed, EWeaponType, WeaponType);

/**
 * 
 */
UCLASS(Blueprintable)
class ARPG_COURSE_API UWeaponEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Arming events")
	FOnWeaponArmed OnWeaponArmed;
};
