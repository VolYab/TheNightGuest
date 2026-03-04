// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Items/Item.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_COURSE_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	FString ItemName;

	UPROPERTY(EditAnywhere)
	FText TextToDisplay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AItem> ItemClass;
	
	// For multiple assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	TArray<FSoftObjectPath> AssetsToLoad;
	
// GETTERS AND SETTERS
public:
	FText GetTextToDisplay() const
	{
	    return TextToDisplay;
	}

	void SetTextToDisplay(const FText& Text)
	{
	    this->TextToDisplay = Text;
	}

	UFUNCTION()
	TSoftObjectPtr<UStaticMesh> GetItemMesh() const
	{
		return ItemMesh;
	}

	TSoftObjectPtr<UTexture2D> GetItemImage() const
	{
	    return ItemImage;
	}

	void SetItemImage(const TSoftObjectPtr<UTexture2D>& Image)
	{
	    this->ItemImage = Image;
	}
};
