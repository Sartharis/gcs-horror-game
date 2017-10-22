// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HexTile.h"
#include "Hex.h"
#include "HexLightComponent.h"
#include "HexMap.generated.h"



//ORIENTATION-------------------------------------------------------------------------
UENUM( BlueprintType )
namespace EHexOrientation
{
	enum Type
	{
		O_POINTY UMETA( DisplayName = "Pointy" ),
		O_FLAT UMETA( DisplayName = "Flat" )
	};
}

/** Just a helper struct to store the data for flat and pointy hex orientations*/
struct Orientation
{
	const double f0, f1, f2, f3;
	const double b0, b1, b2, b3;
	const double start_angle; // in multiples of 60°

	Orientation();

	Orientation( double f0_, double f1_, double f2_, double f3_,
				 double b0_, double b1_, double b2_, double b3_,
				 double start_angle_ )
		: f0( f0_ ), f1( f1_ ), f2( f2_ ), f3( f3_ ),
		b0( b0_ ), b1( b1_ ), b2( b2_ ), b3( b3_ ),
		start_angle( start_angle_ ) {}

	static Orientation GetOrientation( EHexOrientation::Type OrientationEnum );
};

//HEX TILE DATA-------------------------------------------------------------------------------
/** Class that stores the properties for tiles*/
UCLASS()
class UHexTileData : public UObject
{
	GENERATED_BODY()

	public:
	UHexTileData();
	UHexTileData( FHexVector HexLocation_, TSubclassOf<class UAbstractHexTile> HexTileType_ );

	/** Whether the current tile should be treated as a wall*/
	UPROPERTY( BlueprintReadWrite, Category = "Hex Data" )
	bool IsWall;

	/** The location of this hex tile*/
	UPROPERTY( BlueprintReadOnly, Category ="Hex Data" )
	FHexVector HexLocation;

	/** The class that contains all default data of the given tile*/
	UPROPERTY( BlueprintReadOnly, Category = "Hex Data" )
	TSubclassOf<class UAbstractHexTile> HexTileType;

	/** Set that stores all actors currently on the hex tile*/
	UPROPERTY( BlueprintReadOnly, Category = "Hex Data")
	TSet<AActor*> ActorsOnTile;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FLightUpdate, bool, isLit );

// HEX MAP-------------------------------------------------------------------------
/** The class that stores all data about the hex map. ONLY 1 SHOULD EXIST IN A WORLD AT A TIME!*/
UCLASS()
class HEXGAMEFRAMEWORK_API AHexMap : public AActor
{
	GENERATED_BODY()

public:	
	AHexMap();

protected:

	/** Determines whether the hexes are pointy or flat at the top */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Map Settings" )
	TEnumAsByte<EHexOrientation::Type> HexOrientation;

	/** Size in the x axis of the hex*/
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Map Settings" )
	int32 SizeX;

	/** Size in the y axis of the hex*/
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Map Settings" )
	int32 SizeY;

	/**  A map mapping all added hexes to a HexTileData struct. If a coordinate is not in the map, 
	there is no hex at that location */
	UPROPERTY( Instanced, BlueprintReadOnly, Category = "Map Data" )
	TMap<FHexVector, UHexTileData* > HexMap;

	/** A map mapping a static mesh to an instanced mesh component. Used to quickly assign meshes 
	to instanced mesh components */
	UPROPERTY( Instanced )
	TMap<UStaticMesh*, UInstancedStaticMeshComponent* > InstancedMeshComponentMap;

	/** A set containing all currently active hex tile lights on the map*/
	UPROPERTY()
	TSet<UHexLightComponent*> ActiveLights;

	/** Set containing the vectors of all tiles currently lit up by light sources*/
	UPROPERTY()
	TSet<FHexVector> LitTiles;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called whenever object is changed in editor or placed in editor
	virtual void OnConstruction( const FTransform& Transform ) override;

	/** Convert the given hex vector into a location in 3D space (relative to origin of hexmap)*/
	UFUNCTION( BlueprintPure, Category="Coordinates")
	FVector HexVectorToPoint( const FHexVector& Hex ) const;

	/** Get hex vector corresponding to the given point (relative to the map's origin)*/
	UFUNCTION( BlueprintPure, Category="Coordinates" )
	FHexVector PointToHexVector( const FVector& Point ) const;

	/** Add Hex tile to the map or update an existing one */
	UFUNCTION( BlueprintCallable, Category="Map")
	void AddOrChangeHex(const FHexVector& Hex, const TSubclassOf<UAbstractHexTile> HexTile, bool CallUpdateMesh = true );

	/**Remove the hex tile at the given coordinate, if it exists*/
	UFUNCTION( BlueprintCallable, Category="Map" )
	bool RemoveHex( const FHexVector& Hex, bool CallUpdateMesh = true );

	//HEX DATA------------------------------------------------------------------------------------------------
	UFUNCTION( BlueprintPure, Category = "Hex Data" )
	TArray<FHexVector> GetAllTilesOnMap();

	UFUNCTION( BlueprintPure, Category = "Hex Data" )
	UHexTileData* GetDataAtHex( const FHexVector& Hex );

	UFUNCTION( BlueprintCallable, Category = "Hex Data" )
	bool RegisterActorToHex( const FHexVector& Hex, AActor* Actor );

	UFUNCTION( BlueprintCallable, Category = "Hex Data" )
	bool UnregisterActorFromHex( const FHexVector& Hex, AActor* Actor );

	UFUNCTION( BlueprintCallable, Category = "Hex Data" )
	bool TileHasBlockingActor( const FHexVector& Hex, const TArray<TSubclassOf<AActor>> ActorsToIgnore, TArray<AActor*> SpecificActorToIgnore );

	// MESH INSTANCING---------------------------------------------------------------------------------------
	/**Checks if the given static mesh has a corresponding instanced mesh component*/
	UFUNCTION( BlueprintCallable, Category="Instanced Meshes")
	bool HasMeshBeenInstanced( const UStaticMesh* Mesh );

	/**Update the instanced meshes to reflect the changes made in the map. Costly.*/
	UFUNCTION( BlueprintCallable, Category = "Instanced Meshes" )
	void UpdateAllInstancedMeshes();

	//PATHFINDING---------------------------------------------------------------------------------------------
	/**Uses A* algorith to return an array of hex vectors corresponding to the path between hex A and B. 
	   If there is no path, an empty array is returned*/
	UFUNCTION( BlueprintPure, meta = ( Keywords = "pathfinding navigation" ), Category = "Pathfinding" )
	TArray<FHexVector> GetPathBetweenHexes( FHexVector A, FHexVector B, const TArray<TSubclassOf<AActor>> ActorsToIgnore, TArray<AActor*> SpecificActorToIgnore );

	//LIGHTS--------------------------------------------------------------------------------------------------
	/** Sends a broadcast when the component starts its turn*/
	UPROPERTY( BlueprintAssignable, Category = "Light" )
	FLightUpdate LightUpdate;

	void RecalculateLights();

	UFUNCTION( BlueprintImplementableEvent, Category = "Light" )
	void NotifyTileActorsOfLightChange();

	UFUNCTION( BlueprintPure, Category = "Light" )
	bool IsTileLitUp( FHexVector HexLocation );

	void AddLightSource( UHexLightComponent* LightSource);

	void RemoveLightSource( UHexLightComponent* LightSource );

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable, Category = "Light" )
	void UpdateMeshShadows( const TArray<FHexVector>& ChangedTiles );

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable, Category = "Light" )
	void UpdateAllMeshShadows();
};
