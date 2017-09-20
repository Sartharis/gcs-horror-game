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

	//The mesh representing the object, all components should be attached to this
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* RootMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called whenever object is changed in editor or placed in editor
	virtual void OnConstruction( const FTransform& Transform ) override;

	
	
};
