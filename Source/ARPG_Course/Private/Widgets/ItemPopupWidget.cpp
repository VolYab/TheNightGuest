// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemPopupWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataAssets/ItemDataAsset.h"

void UItemPopupWidget::SendItemData_Implementation(UItemDataAsset* ItemDataAsset)
{
	if (ItemDataAsset)
	{
		PopupText->SetText(ItemDataAsset->GetTextToDisplay());
        PopupImage->SetBrushFromSoftTexture(ItemDataAsset->GetItemImage());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataAsset is null"));
	}
	
}
