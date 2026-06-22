// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Sword UMETA(DisplayName = "Sword"),
	EWT_Axe UMETA(DisplayName = "Axe"),
	EWT_Spear UMETA(DisplayName = "Spear")
};

UENUM(BlueprintType)
enum class EGripType : uint8
{
	EGT_1Hand UMETA(DisplayName = "One-Handed Grip"),
	EGT_2Hand UMETA(DisplayName = "Two-Handed Grip")
};

UCLASS()
class ARPG_COURSE_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	TArray<AActor*> IgnoreActors;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGripType GripType;
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
private:
	UPROPERTY(EditAnywhere)
	USoundBase* EquipSound;
	
	UPROPERTY(VisibleAnywhere, Category = "Box properties")
	UBoxComponent* WeaponBoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Box properties")
	USceneComponent* TraceBoxStart;

	UPROPERTY(VisibleAnywhere, Category = "Box properties")
	USceneComponent* TraceBoxEnd;

	UPROPERTY(EditAnywhere, Category = "Box properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Box properties")
	bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere, Category = "Attack properties")
	float WeaponDamage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Attack properties")
	float AttackRange = 100.f;

	void PlayEquipSound() const;
	void DisableCollisionSphere() const;
	void HidePopupWidget() const;
	void BoxTrace(FHitResult& BoxHit);
	void HandleHitInteraction(FHitResult BoxHit);
	bool ActorIsSameType(const AActor* OtherActor) const;
public:
	//Getters and Setters
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EGripType GetGripType() const { return GripType; }
	FORCEINLINE UBoxComponent* GetWeaponBoxComponent() const {return WeaponBoxComponent;}

	FName GetGripName() const;
};
