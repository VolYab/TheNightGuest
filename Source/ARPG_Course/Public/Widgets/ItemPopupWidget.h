// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/ItemWidgetInterface.h"
#include "ItemPopupWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_COURSE_API UItemPopupWidget : public UUserWidget, public IItemWidget
{
	GENERATED_BODY()

public:
	virtual void SendItemData_Implementation(UItemDataAsset* ItemDataAsset) override;
	
protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PopupText;

	UPROPERTY(meta=(BindWidget))
	class UImage* PopupImage;

	
};
