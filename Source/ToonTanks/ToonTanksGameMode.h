// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	public:

		void ActorDied(AActor* DeadActor);

		UFUNCTION(BlueprintCallable)
		int32 GetTimer() const;
		
	protected:

		virtual void BeginPlay() override;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GamePlay", meta = (AllowPrivateAccess = "true"))
		bool bPlayerHasWon = false;

		bool bGameHasEnded = false;
		
	private:

		void HandleGameStart();
		void HandleGameOver(const bool& PlayerWon);
		void UpdateTurretAmount();
		void SetupWidgets();
		void GameStartTimerFunction();

		UPROPERTY(EditDefaultsOnly, Category = "Turrets")
		TSubclassOf<class APawnTurret> TurretClass;

		UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> GameStartWidgetClass;
		UPROPERTY()
		class UUserWidget* GameStartWidget = nullptr;

		UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> GameEndWidgetClass;
		UPROPERTY()
		class UUserWidget* GameEndWidget = nullptr;

		UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> PlayerHubWidgetClass;
		UPROPERTY()
		class UUserWidget* PlayerHubWidget = nullptr;

		UPROPERTY(EditAnywhere, Category = "Settings")
		int32 GameStartTimer = 3;

		class APlayerControllerBase* PC;

		int32 TargetTurrets = 0;
		TArray<AActor*>TurretArray;
		FTimerHandle GameStartTimerHandle;


};
