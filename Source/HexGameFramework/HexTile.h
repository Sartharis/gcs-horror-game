#pragma once

#include "HexGameFramework.h"
#include "HexTile.generated.h"

/**
 * The base class for all Hex Tiles (Note: these classes are never instanced)
 */
UCLASS( Abstract, Blueprintable, BlueprintType )
class HEXGAMEFRAMEWORK_API UAbstractHexTile : public UObject
{
	GENERATED_BODY()

public:
	UAbstractHexTile();

	/** How expensive it is to move onto this tile. A negative cost means infinite cost (i.e. you can't move onto this tile)*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Movement")
	bool IsWall;

	/** The mesh that is used to represent the tile. The Hex Map system creates an instanced component for each unique mesh*/
	UPROPERTY( BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh" )
	UStaticMesh* TileMesh;

	/** How high should the selector be when hovering over this tile?*/
	UPROPERTY( BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh" )
	float SelectionDrawHeight;

};

UCLASS()
class ADropTest : public AActor
{
	GENERATED_BODY()

public:
	ADropTest();

	void OnObjectSelected( UObject* Object );
};