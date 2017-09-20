// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGameFramework.h"
#include "HexMap.h"
#include "HexFunctionLibrary.h"
#include "TileObject.h"


// Sets default values
ATileObject::ATileObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>( TEXT( "RootComponent" ) );
	RootComponent = Scene;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Mesh" ) );
	RootMesh->SetupAttachment( RootComponent );
}

// Called when the game starts or when spawned
void ATileObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATileObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileObject::OnConstruction( const FTransform& Transform )
{
	Super::OnConstruction( Transform );
	
	
}

