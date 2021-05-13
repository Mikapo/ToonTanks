// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"


APawnTank::APawnTank()
{
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PrimaryActorTick.bCanEverTick = true;
}

void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsDead) return;
	if (!PC) return;

	Rotate();
	Move();

	FVector MouseLocation = GetHitUnderPlayerCursor().ImpactPoint;
	LookAtTarget(MouseLocation, DeltaTime);
}

void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMovementInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Shoot);
}

void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController not found on PlayerTank"))
		return;
	}

	PC->bShowMouseCursor = true;
}

void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();

	BaseMesh->SetVisibility(false);
	TurretMesh->SetVisibility(false);
}

void APawnTank::CalculateMovementInput(float Input)
{
	MovementDirection = FVector(Input * MoveSpeed * GetWorld()->GetDeltaSeconds(), 0, 0 );
}

void APawnTank::CalculateRotateInput(float Input)
{
	float RotationAmount = Input * TurnSpeed * GetWorld()->GetDeltaSeconds();
	FRotator Rotation = FRotator(0, RotationAmount, 0);
	RotationDirection = FQuat(Rotation);
}

void APawnTank::Move()
{
	if (bIsDead) return;
	AddActorLocalOffset(MovementDirection, true);
}

void APawnTank::Rotate()
{
	if (bIsDead) return;
	AddActorLocalRotation(RotationDirection, true);
}

FHitResult APawnTank::GetHitUnderPlayerCursor() const
{
	FHitResult Hit;
	if (!PC) return Hit;

	PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	return Hit;
}
