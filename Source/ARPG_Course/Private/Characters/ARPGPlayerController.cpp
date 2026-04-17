// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ARPGPlayerController.h"

AARPGPlayerController::AARPGPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetGenericTeamId(FGenericTeamId(2));
}

void AARPGPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	if (TeamId != NewTeamId)
	{
		TeamId = NewTeamId;
	}
}
