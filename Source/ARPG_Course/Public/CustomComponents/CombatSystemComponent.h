// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatSystemComponent.generated.h"

enum class EWeaponType : uint8;
class UChooserTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ARPG_COURSE_API UCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatSystemComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetWeaponType(EWeaponType NewWeaponType);

	UFUNCTION()
	UAnimMontage* SelectAssetFromChooser();

	float GetComboCount() const
	{
		return ComboCount;
	}

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UChooserTable* WeaponChooserTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ComboCount = 0;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void IncreaseComboCount();

	UFUNCTION(BlueprintCallable)
	void ResetComboCount();

private:
	void ResetComboCD();
};
