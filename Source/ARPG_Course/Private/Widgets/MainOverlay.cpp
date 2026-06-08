#include "Widgets/MainOverlay.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMainOverlay::SetHealthPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UMainOverlay::SetStaminaPercent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

void UMainOverlay::SetPlayerName(FString Name)
{
	if (PlayerTextName)
	{
		PlayerTextName->SetText(FText::FromString(Name));
	}
}

void UMainOverlay::SetPlayerIcon(UTexture2D* Icon)
{
	if (PlayerIcon)
	{
		PlayerIcon->SetBrushFromTexture(Icon);
	}
}
