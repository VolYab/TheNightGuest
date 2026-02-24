// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "ARPG_Course/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/ARPGCharacter.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/ItemDataAsset.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/ItemWidgetInterface.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(GetRootComponent());

	PopupWidget = CreateDefaultSubobject<UWidgetComponent>("PopupWidget");
	PopupWidget->SetupAttachment(GetRootComponent());
	FVector WidgetLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 100.f);
	PopupWidget->SetWorldLocation(WidgetLocation);
	PopupWidget->SetWidgetSpace(EWidgetSpace::Screen);
	PopupWidget->SetTickMode(ETickMode::Automatic);
	PopupWidget->SetDrawSize(FVector2D(100.f, 100.f));
	PopupWidget->SetWidgetClass(UUserWidget::StaticClass());
	
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
		UE_LOG(LogTemp, Warning, TEXT("Overlap!"))
		
		if (PopupWidgetClass)
		{
			PopupWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), PopupWidgetClass);
			if (PopupWidgetInstance)
			{
				PopupWidgetInstance->AddToViewport();
				PopupWidgetInstance->SetVisibility(ESlateVisibility::Visible);
				/*if (IItemWidget* ItemWidgetInterface = Cast<IItemWidget>(PopupWidgetInstance))
				{
					ItemWidgetInterface->Execute_SetVisibility(PopupWidgetInstance, true);
					if (ItemData)
					{
						ItemWidgetInterface->Execute_SendItemData(PopupWidgetInstance, ItemData);
					}
				}*/
			}
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
		if (PopupWidgetInstance)
		{
			PopupWidgetInstance->RemoveFromParent();
			PopupWidgetInstance = nullptr;
		}
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	
}

