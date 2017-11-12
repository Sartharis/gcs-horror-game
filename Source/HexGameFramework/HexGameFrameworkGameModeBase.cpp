// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGameFramework.h"
#include "HexGameFrameworkGameModeBase.h"




AHexGameFrameworkGameModeBase::AHexGameFrameworkGameModeBase()
{
}

UTurnManager* AHexGameFrameworkGameModeBase::GetTurnManager()
{
	if( TurnManager == NULL )
	{
		TurnManager = NewObject<UTurnManager>();
	}
	return TurnManager;
}

void AHexGameFrameworkGameModeBase::WorldCollisionChange()
{
	WorldCollisionChangeEvent.Broadcast();
}

