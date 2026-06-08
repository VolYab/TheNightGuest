// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ST_HUD.generated.h"

class UMainOverlay;

/**
 * HUD class used to manage and display the main player interface.
 * Includes functionality to initialize widgets like the MainOverlay.
 */
UCLASS()
class ARPG_COURSE_API AST_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UMainOverlay> MainOverlayClass;

	UPROPERTY()
	UMainOverlay* MainOverlay;
};
