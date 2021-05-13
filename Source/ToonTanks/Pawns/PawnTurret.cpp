// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "TimerManager.h"
#include "PawnTank.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "GameFramework/SpringArmComponent.h"


APawnTurret::APawnTurret()
{
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(RootComponent);
	HealthBar->SetVisibility(false);

	OnTakeAnyDamage.AddDynamic(this, &APawnTurret::ShowHealthBar);
}

void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!PlayerTank) return;

	if (GetDistanceOfPlayer() < Range)
	{
		LookAtTarget(PlayerTank->GetActorLocation(), DeltaTime);
	}

	else if(InitialTurretRotation != TurretMesh->GetComponentRotation())
	{
		RotateTurret(InitialTurretRotation, DeltaTime);
	}

}

void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	InitialTurretRotation = TurretMesh->GetComponentRotation();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimeHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);

	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Error, TEXT("Player tank not found on level"))
	}

	USpringArmComponent* SpringArm = FindComponentByClass<USpringArmComponent>();

	if(SpringArm) UE_LOG(LogTemp, Warning, TEXT("%s"), SpringArm->GetName())
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void APawnTurret::CheckFireCondition()
{
	if (!PlayerTank) return;
	if (PlayerTank->IsItDead())return;

	if (GetDistanceOfPlayer() < Range && IsPlayerInVision() && GetAngleTowardsPlayer() < 30.0f)
	{
		Shoot();
	}

}

void APawnTurret::ShowHealthBar(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	HealthBar->SetVisibility(true);
	GetWorld()->GetTimerManager().SetTimer(HideHealthBarTimerHandle, this, &APawnTurret::HideHealthBar, 1.0f, false);
}

void APawnTurret::HideHealthBar()
{
	HealthBar->SetVisibility(false);
}

float APawnTurret::GetDistanceOfPlayer() const
{
	if (!PlayerTank) return 0.0f;

	return FVector::Dist(PlayerTank->GetActorLocation(), GetActorLocation());

}

float APawnTurret::GetAngleTowardsPlayer() const
{
	if (!PlayerTank) return 0.0f;

	const FVector TurretPosition = FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f);
	const FVector PlayerPosition = FVector(PlayerTank->GetActorLocation().X, PlayerTank->GetActorLocation().Y, 0.0f);

	FVector UnitVectorTowardsPlayer = PlayerPosition - TurretPosition;
	UnitVectorTowardsPlayer.Normalize();

	return FMath::RadiansToDegrees(acosf(FVector::DotProduct(TurretMesh->GetForwardVector(), UnitVectorTowardsPlayer)));
}

bool APawnTurret::IsPlayerInVision() const
{
	if (!PlayerTank) return false;

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), PlayerTank->GetActorLocation(), ECollisionChannel::ECC_Visibility);

	if (Hit.GetActor() == PlayerTank) return true;
	else return false;
	
}
