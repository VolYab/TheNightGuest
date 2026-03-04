// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/ARPGCharacter.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Breakable/BreakableActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"

AWeapon::AWeapon()
{
	//Add collision box to register hits
	WeaponBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box Component"));
	WeaponBoxComponent->SetupAttachment(GetRootComponent());

	//Set collision box start point
	TraceBoxStart = CreateDefaultSubobject<USceneComponent>(TEXT("Trace Box Start"));
	TraceBoxStart->SetupAttachment(GetRootComponent());

	//Set collision box end point
	TraceBoxEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Trace Box End"));
	TraceBoxEnd->SetupAttachment(GetRootComponent());

	//Configure collision settings
	WeaponBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

/*
 * This function attaches item to character mesh
 */
void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	StaticMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

/*
 * This function equips item from world
 */
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	//Play sound when equip
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
	}
	if (CollisionSphere)
	{
		 CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	PopupWidget->SetVisibility(false);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

/*
 * This function generates Box Collision Trace to check Weapon hits
 */
void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector Start = TraceBoxStart->GetComponentLocation();
	FVector End = TraceBoxEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	
	FHitResult HitResult;
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		TraceBoxStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
		);

	if (HitResult.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());
        if (HitInterface)
        {
        	HitInterface->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint);
        	CreateFields(HitResult.ImpactPoint);
        }
		IgnoreActors.AddUnique(HitResult.GetActor());
		
	}
}
