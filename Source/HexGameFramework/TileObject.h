// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TileObject.generated.h"


UCLASS()
class HEXGAMEFRAMEWORK_API ATileObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileObject();

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Tile Collision")
	bool BlockActors;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Tile Collision" )
	TArray<TSubclassOf<AActor>> ActorsToIgnoreBlockFrom;
};
