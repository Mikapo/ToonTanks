// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
	GENERATED_BODY()

	public:
		// Sets default values for this pawn's properties
		APawnTank();

		// Called every frame
		virtual void Tick(float DeltaTime) override;

		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		virtual void HandleDestruction() override;


	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	private:

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm = nullptr;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera = nullptr;

		class APlayerController* PC; 

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
		float MoveSpeed = 100;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
		float TurnSpeed = 10;

		FVector MovementDirection;
		FQuat RotationDirection;

		void CalculateMovementInput(float Input);
		void CalculateRotateInput(float Input);
		void Move();
		void Rotate();
		FHitResult GetHitUnderPlayerCursor() const;



};
