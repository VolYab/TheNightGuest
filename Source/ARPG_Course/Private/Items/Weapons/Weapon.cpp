// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Perception/AISense_Damage.h"
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
 * This function equips item from the world
 */
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableCollisionSphere();
	PlayEquipSound();
	HidePopupWidget();
}

/*
 * This function generates Box Collision Trace to check Weapon hits
 */
void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorIsSameType(OtherActor))
	{
		return;
	}
	
	FHitResult BoxHitResult;
	BoxTrace(BoxHitResult);

	AActor* DamagedActor = BoxHitResult.GetActor();
	AActor* DamageInstigator = GetInstigator();
	if (DamagedActor)
	{
		if (ActorIsSameType(DamagedActor))
		{
			return;
		}
		UGameplayStatics::ApplyDamage(DamagedActor, WeaponDamage, GetInstigatorController(), this, UDamageType::StaticClass());

		// Notify the AI Perception System of the damage event
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),
			DamagedActor,
			DamageInstigator,
			WeaponDamage,
			DamageInstigator->GetActorLocation(),
			BoxHitResult.ImpactPoint
		);
		
		HandleHitInteraction(BoxHitResult);
		CreateFields(BoxHitResult.ImpactPoint);
	}
}

void AWeapon::PlayEquipSound() const
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
	}
}

void AWeapon::DisableCollisionSphere() const
{
	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::HidePopupWidget() const
{
	PopupWidget->SetVisibility(false);
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	FVector Start = TraceBoxStart->GetComponentLocation();
	FVector End = TraceBoxEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		TraceBoxStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
		);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}

void AWeapon::HandleHitInteraction(FHitResult BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
	}
}

FName AWeapon::GetGripName() const
{
	return GripType == EGripType::EGT_1Hand ? FName("1HWeapon") : FName("2HWeapon");
}

bool AWeapon::ActorIsSameType(const AActor* OtherActor) const
{
	return GetOwner()->ActorHasTag(FName("Enemy")) && OtherActor->ActorHasTag(FName("Enemy"));
}
