// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

class UItemDataAsset;
class AItem;

UCLASS()
class ARPG_COURSE_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemSpawner();
	virtual void Tick(float DeltaTime) override;

	/**
	 * This function spawns synchronously items based on DataAsset provided
	 * @param WorldContextObject WCO is needed for static function in order to call GetWorld()
	 * @param ItemData Data asset to use for item spawning
	 * @return Spawned item
	 */
	UFUNCTION(meta = (WorldContext = "WorldContextObject"))
	static AItem* SpawnItem(UObject* WorldContextObject, UItemDataAsset* ItemData);

protected:
	virtual void BeginPlay() override;
	
};
