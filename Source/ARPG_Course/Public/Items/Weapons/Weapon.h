// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_1HSword UMETA(DisplayName = "One-Handed Sword"),
	EWT_2HSword UMETA(DisplayName = "Two-Handed Sword")
};

UCLASS()
class ARPG_COURSE_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	void Equip(USceneComponent* InParent, FName InSocketName);

	TArray<AActor*> IgnoreActors;

protected:
	void BeginPlay() override;
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
private:
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType = EWeaponType::EWT_1HSword;

	UPROPERTY(EditAnywhere, Category = "Weapon properties")
	USoundBase* EquipSound;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon properties")
	UBoxComponent* WeaponBoxComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* TraceBoxStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* TraceBoxEnd;
//Getters and Setters
public:
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE UBoxComponent* GetWeaponBoxComponent() const {return WeaponBoxComponent;}
};
