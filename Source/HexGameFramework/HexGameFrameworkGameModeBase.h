// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "TurnManager.h"
#include "HexMap.h"
#include "HexGameFrameworkGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HEXGAMEFRAMEWORK_API AHexGameFrameworkGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHexGameFrameworkGameModeBase();

	UFUNCTION( BlueprintPure, Category = "Turn Manager" )
	UTurnManager* GetTurnManager();

	UPROPERTY( BlueprintReadWrite, Category = "Hex Map" )
	AHexMap* CurrentHexMap;

	UPROPERTY( BlueprintReadWrite, Category = "Hex Map" )
	AActor* PlayerController;


private:
	UTurnManager* TurnManager;
	
	
	
};
