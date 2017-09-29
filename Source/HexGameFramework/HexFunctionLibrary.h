// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HexGameFramework.h"
#include "HexGameFrameworkGameModeBase.h"
#include "Hex.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HexFunctionLibrary.generated.h"

UCLASS( )
class HEXGAMEFRAMEWORK_API UHexFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	/** Round the given fractional cube hex coordinates to the nearest hex */
	UFUNCTION( BlueprintPure, Category="Hex Coordinates")
	static FHexVector HexRound( float Q, float R, float S );

	/** Returns an array consisting of all hexes from A and B (in a straight line)*/
	UFUNCTION( BlueprintPure, Category = "Hex Coordinates" )
	static TArray<FHexVector> HexLine( FHexVector A, FHexVector B );

	/** Returns an array consisting of all hexes in a ring of some radius around some center hex */
	UFUNCTION( BlueprintPure, Category = "Hex Coordinates" )
	static TArray<FHexVector> HexRing( FHexVector Center, int Radius );

	/** Return the default data object of the type of hex tile at the given position.
		Can be NULL, remember to check if the object is valid!*/
	//UFUNCTION( BlueprintPure, Category = "Hex Coordinates" )
	static UAbstractHexTile* GetTileTypeObject( AHexMap* HexMap, FHexVector Tile );

	/** Returns an array consisting of all hexes in a radius from the center*/
	UFUNCTION( BlueprintPure, Category = "Hex Coordinates" )
	static TArray<FHexVector> HexInRadius( FHexVector Center, int Radius );

	/* Hex Vector equality */
	UFUNCTION( BlueprintPure, meta = ( DisplayName = "hexvector == hexvector", CompactNodeTitle = "==", Keywords = "= equal same" ), Category = "Hex Coordinates" )
	static bool Equal_HexVectorHexVector( FHexVector A, FHexVector B );

	/* Hex Vector addition */
	UFUNCTION( BlueprintPure, meta = ( DisplayName = "hexvector + hexvector", CompactNodeTitle = "+", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true" ), Category = "Hex Coordinates" )
	static FHexVector Add_HexVectorHexVector( FHexVector A, FHexVector B );

	/* Hex Vector subtraction */
	UFUNCTION( BlueprintPure, meta = ( DisplayName = "hexvector - hexvector", CompactNodeTitle = "-", Keywords = "- subtract minus" ), Category = "Hex Coordinates" )
	static FHexVector Subtract_HexVectorHexVector( FHexVector A, FHexVector B );

	/* Scales Hex Vector A by B */
	UFUNCTION( BlueprintPure, meta = ( DisplayName = "hexvector * float", CompactNodeTitle = "*", Keywords = "* multiply" ), Category = "Hex Coordinates" )
	static FHexVector Multiply_HexVectorFloat( FHexVector A, float B );

	/* Returns the length of the FHexVector */
	UFUNCTION( BlueprintPure, meta = ( DisplayName = "Hex Vector Length", Keywords = "magnitude" ), Category = "Hex Coordinates" )
	static int HSize( FHexVector A );

	/* Returns the distance between two FHexVectors */
	UFUNCTION( BlueprintPure, meta = ( DisplayName = "Hex Vector Distance", Keywords = "distance" ), Category = "Hex Coordinates" )
	static int HDist( FHexVector A, FHexVector B);

	/* Returns the distance between two FHexVectors */
	UFUNCTION( BlueprintPure, meta = ( DisplayName = "Make Hex Vector Axial", Keywords = "make" ), Category = "Hex Coordinates" )
	static FHexVector MakeHexVectorAxial( int32 X, int32 Y );

	static AHexGameFrameworkGameModeBase* GetHexGameMode( const UObject* WorldContextObject );
};
