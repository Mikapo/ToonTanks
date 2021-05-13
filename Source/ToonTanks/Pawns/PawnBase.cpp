// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

}

void APawnBase::LookAtTarget(const FVector& Target, const float& DeltaTime)
{
	const FVector LookAtTargetClean = FVector(Target.X, Target.Y, TurretMesh->GetComponentLocation().Z);
	const FVector StartLocation = TurretMesh->GetComponentLocation();

	FRotator Rotation = FVector(LookAtTargetClean - StartLocation).Rotation();
	TurretMesh->SetWorldRotation(UKismetMathLibrary::RInterpTo_Constant(TurretMesh->GetComponentRotation(), Rotation, DeltaTime, RotationSpeed));
}

void APawnBase::RotateTurret(const FRotator& Rotation, const float& DeltaTime)
{
	TurretMesh->SetWorldRotation(UKismetMathLibrary::RInterpTo_Constant(TurretMesh->GetComponentRotation(), Rotation, DeltaTime, RotationSpeed));
}

void APawnBase::Shoot()
{
	if (bIsDead) return;
	if (!ProjectileClass) return;

	const FVector ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	const FRotator ProjectileSpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation);
	Projectile->SetOwner(this);
	

}

void APawnBase::HandleDestruction()
{
	bIsDead = true;

	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(this, DeadParticle, GetActorLocation());
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeadShakeClass);
}

bool APawnBase::IsItDead()
{
	return bIsDead;
}

void APawnBase::BeginPlay()
{
	Super::BeginPlay();

	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile class not found on %s"), *GetName())
	}
}
