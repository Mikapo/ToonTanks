// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "ToonTanks/Controllers/PlayerControllerBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"





void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!TurretClass)
	{
		UE_LOG(LogTemp, Error, TEXT("turret class has not been assigned in ToonTanksGameMode"))
	}

	UpdateTurretAmount();
	SetupWidgets();
	HandleGameStart();

	
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (APawnTurret* Turret = Cast<APawnTurret>(DeadActor))
	{
		TurretArray.Remove(Turret);
		Turret->HandleDestruction();
		UpdateTurretAmount();

		if (TargetTurrets == 0)
		{
			HandleGameOver(true);
		}
	}

	else if (APawnTank* PlayerTank = Cast<APawnTank>(DeadActor))
	{
		PlayerTank->HandleDestruction();
		HandleGameOver(false);
	}
}

void AToonTanksGameMode::HandleGameStart()
{
	if (!GameStartWidget || !PlayerHubWidget) return;

	GameStartWidget->AddToViewport();
	PlayerHubWidget->AddToViewport();

	PC = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController());

	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s did not find the player controller"), *GetName())
		return;
	}

	PC->SetInputEnabledState(false);
	GetWorld()->GetTimerManager().SetTimer(GameStartTimerHandle, this, &AToonTanksGameMode::GameStartTimerFunction, 1.0f, true);


}

void AToonTanksGameMode::GameStartTimerFunction()
{
	if (GameStartTimer == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(GameStartTimerHandle);
		PC->SetInputEnabledState(true);
		GameStartWidget->RemoveFromViewport();
	}
	
	else
	{
		GameStartTimer--;
	}
}

void AToonTanksGameMode::HandleGameOver(const bool& PlayerWon)
{
	if (!GameEndWidget || !PC) return;

	bGameHasEnded = true;
	bPlayerHasWon = PlayerWon;
	GameEndWidget->AddToViewport();
	PC->SetInputEnabledState(false);
}

void AToonTanksGameMode::UpdateTurretAmount()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretArray);
	TargetTurrets = TurretArray.Num();
}

void AToonTanksGameMode::SetupWidgets()
{
	if (GameStartWidgetClass)
	{
		GameStartWidget = CreateWidget<UUserWidget>(GetWorld(), GameStartWidgetClass);
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameStartWidgetClass has not been set in %s"), *GetName())
	}

	if (GameEndWidgetClass)
	{
		GameEndWidget = CreateWidget<UUserWidget>(GetWorld(), GameEndWidgetClass);
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameEndWidgetClass has not been set in %s"), *GetName())
	}

	if (PlayerHubWidgetClass)
	{
		PlayerHubWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHubWidgetClass);
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerHubWidgetClass has not been set in %s"), *GetName())
	}

}

int32 AToonTanksGameMode::GetTimer() const
{
	return GameStartTimer;
}