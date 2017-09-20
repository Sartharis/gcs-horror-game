// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGameFramework.h"
#include "HexGameFrameworkGameModeBase.h"




AHexGameFrameworkGameModeBase::AHexGameFrameworkGameModeBase()
{
	TurnManager = NewObject<UTurnManager>();
}

UTurnManager* AHexGameFrameworkGameModeBase::GetTurnManager()
{
	return TurnManager;
}

