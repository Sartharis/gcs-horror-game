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
	
	/* Hex Vector equality */
	UFUNCTION( BlueprintPure, meta = ( DisplayName = "hexvector == hexvector", CompactNodeTitle = "==", Keywords = "= equal same", CommutativeAssociativeBinaryOperator = "true" ), Category = "Hex Coordinates" )
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
