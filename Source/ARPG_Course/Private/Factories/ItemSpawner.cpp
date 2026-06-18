// Fill out your copyright notice in the Description page of Project Settings.


#include "Factories/ItemSpawner.h"
#include "DataAssets/ItemDataAsset.h"
#include "HAL/Platform.h"
#include "Items/Item.h"

AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AItem* AItemSpawner::SpawnItem(UObject* WorldContextObject, UItemDataAsset* ItemData)
{
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemData is not provided for ItemSpawner!"));
		return nullptr;
	}

	const TSoftClassPtr<AItem> ItemClassPtr = ItemData->GetItemClass();
	if (ItemClassPtr.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemData is invalid."));
		return nullptr;
	}

	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid WorldContextObject provided to SpawnItem."));
		return nullptr;
	}

	// Synchronously load the Item class so we can return the spawned item from this call.
	UClass* LoadedClass = ItemClassPtr.LoadSynchronous();
	if (!LoadedClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load Item class."));
		return nullptr;
	}

	const FVector Location(0.0f, 0.0f, 0.0f);
	const FRotator Rotation(0.0f, 0.0f, 0.0f);

	FTransform SpawnTransform = FTransform(Rotation, Location);
	// Spawn the Item
	AItem* SpawnedItem = WorldContextObject->GetWorld()->SpawnActorDeferred<AItem>(LoadedClass, SpawnTransform);
	if (!SpawnedItem)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn the item."));
		return nullptr;
	}

	// Assign the ItemData property
	SpawnedItem->SetItemData(ItemData);
	//Finish spawning
	SpawnedItem->FinishSpawning(FTransform(Rotation, Location));
	//UGameplayStatics::FinishSpawningActor(SpawnedItem, FTransform(Rotation, Location));
	
	return SpawnedItem;
}
