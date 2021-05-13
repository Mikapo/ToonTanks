// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

void APlayerControllerBase::SetInputEnabledState(bool Enabled)
{
	if (Enabled)
	{
		GetPawn()->EnableInput(this);
	}

	else
	{
		GetPawn()->DisableInput(this);
	}
}

