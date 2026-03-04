// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class ARPG_COURSE_API ABreakableActor : public AActor, public IHitInterface 
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsBroken = false;

private:
	UPROPERTY(EditAnywhere, Category = "Treasures")
	TArray<TSubclassOf<class ATreasure>> SpawnedTreasures;

/* GETTERS AND SETTERS */
public:
	bool GetIsBroken() const
	{
		return bIsBroken;
	}

	void SetIsBroken(bool bBroken)
	{
		this->bIsBroken = bBroken;
	}	
};
