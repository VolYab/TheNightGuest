// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ST_HUD.h"
#include "Widgets/MainOverlay.h"

void AST_HUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null"));
		return;
	}
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null"));
		return;
	}
	MainOverlay = CreateWidget<UMainOverlay>(PlayerController, MainOverlayClass);
	if (MainOverlay)
	{
		MainOverlay->AddToViewport();
	}
}
