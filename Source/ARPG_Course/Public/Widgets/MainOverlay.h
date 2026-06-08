// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainOverlay.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;

UCLASS()
class ARPG_COURSE_API UMainOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);
	void SetStaminaPercent(float Percent);
	void SetPlayerName(FString Name);
	void SetPlayerIcon(UTexture2D* Icon);

private:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta=(BindWidget))
	UImage* MainBarsOverlay;

	UPROPERTY(meta=(BindWidget))
	UImage* PlayerIcon;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayerTextName;
};
