// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	public:	
		// Sets default values for this component's properties
		UHealthComponent();

	protected:
		// Called when the game starts
		virtual void BeginPlay() override;

		UFUNCTION()
		void TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	private:

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
		float DefaultHealth = 100.0f;

		UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
		float Health = 0.0f;

		class AToonTanksGameMode* GM;

};
