// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTurret.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase
{
	GENERATED_BODY()

	public:

		// Sets default values for this pawn's properties
		APawnTurret();

		// Called every frame
		virtual void Tick(float DeltaTime) override;

		virtual void HandleDestruction() override;

	protected:

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UWidgetComponent* HealthBar = nullptr;

	private:

		UFUNCTION()
		void ShowHealthBar(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

		void HideHealthBar();
		void CheckFireCondition();
		float GetDistanceOfPlayer() const;
		float GetAngleTowardsPlayer() const;
		bool IsPlayerInVision() const;
		
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
		float FireRate = 2.0f;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
		float Range = 800;

		FTimerHandle FireRateTimeHandle;
		FTimerHandle HideHealthBarTimerHandle;

		FRotator InitialTurretRotation;

		class APawnTank* PlayerTank;

		

	
};
