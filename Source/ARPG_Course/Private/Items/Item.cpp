// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "ARPG_Course/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/ARPGCharacter.h"
#include "Components/WidgetComponent.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(GetRootComponent());

	PopupWidget = CreateDefaultSubobject<UWidgetComponent>("PopupWidget");
	PopupWidget->SetupAttachment(GetRootComponent());
	
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	//Add bind a callback function to a delegate
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AARPGCharacter* ARPGCharacter = Cast<AARPGCharacter>(OtherActor);
	if (ARPGCharacter)
	{
		ARPGCharacter->SetOverlappingItem(this);
		if (PopupWidget && ItemState != EItemState::EIS_Equipped)
		{
			PopupWidget->SetVisibility(true);
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AARPGCharacter* ARPGCharacter = Cast<AARPGCharacter>(OtherActor);
	if (ARPGCharacter)
	{
		ARPGCharacter->SetOverlappingItem(nullptr);
		if (PopupWidget)
		{
			PopupWidget->SetVisibility(false);
		}
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	
}