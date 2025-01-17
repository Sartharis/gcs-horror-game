// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGameFramework.h"
#include "HexFunctionLibrary.h"
#include "HexMap.h"

Orientation Orientation::GetOrientation( EHexOrientation::Type OrientationEnum )
{
	if( OrientationEnum == EHexOrientation::O_FLAT )
	{
		return Orientation( 3.0 / 2.0, 0.0, FMath::Sqrt( 3.0 ) / 2.0, FMath::Sqrt( 3.0 ),
							2.0 / 3.0, 0.0, -1.0 / 3.0, FMath::Sqrt( 3.0 ) / 3.0, 0.0 );
	}
	else
	{
		return Orientation( FMath::Sqrt( 3.0 ), FMath::Sqrt( 3.0 ) / 2.0, 0.0, 3.0 / 2.0,
							FMath::Sqrt( 3.0 ) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5 );
	}
}

UHexTileData::UHexTileData( )

{
}

UHexTileData::UHexTileData( FHexVector HexLocation_, TSubclassOf<class UAbstractHexTile> HexTileType_ )
	: HexLocation(HexLocation_), HexTileType(HexTileType_)
{
}

AHexMap::AHexMap() 
	: SizeX(64), SizeY(64), HexOrientation( EHexOrientation::O_POINTY )
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHexMap::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHexMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHexMap::OnConstruction( const FTransform& Transform )
{
	Super::OnConstruction( Transform );

}

FVector AHexMap::HexVectorToPoint( const FHexVector& Hex ) const
{
	const Orientation& M = Orientation::GetOrientation(HexOrientation);
	float X = ( M.f0 * Hex.Q + M.f1 * Hex.R ) * SizeX;
	float Y = ( M.f2 * Hex.Q + M.f3 * Hex.R ) * SizeY;
    const FVector& Loc = GetActorLocation();
	return FVector( X + Loc.X, Y + Loc.Y, Loc.Z );
}

FHexVector AHexMap::PointToHexVector( const FVector& Point ) const
{
	const Orientation& M = Orientation::GetOrientation( HexOrientation );
	const FVector& Loc = GetActorLocation();
	FVector2D pt = FVector2D( ( Point.X - Loc.X ) / SizeX,
							  ( Point.Y - Loc.Y ) / SizeY );
	float q = M.b0 * pt.X + M.b1 * pt.Y;
	float r = M.b2 * pt.X + M.b3 * pt.Y;
	return UHexFunctionLibrary::HexRound( q, r, -q - r );
}

void AHexMap::AddOrChangeHex( const FHexVector& Hex, const TSubclassOf<UAbstractHexTile> HexTile, bool CallUpdateMesh /*= true */ )
{
	UHexTileData** TilePtr = HexMap.Find( Hex );

	/** We check if there is an instanced mesh component for this tile. If not, we add a new instanced mesh component for our new mesh.*/
	UStaticMesh* TileMesh = ( Cast<UAbstractHexTile>( HexTile->GetDefaultObject() ) )->TileMesh;
	if( !HasMeshBeenInstanced( TileMesh ))
	{
		//Adding and registering new component
		UInstancedStaticMeshComponent *ISMComp = NewObject<UInstancedStaticMeshComponent>( this );
		ISMComp->RegisterComponent();
		ISMComp->SetStaticMesh( TileMesh );
		AddInstanceComponent( ISMComp );

		//Adding it to our instanced mesh array
		InstancedMeshComponentMap.Add( TileMesh, ISMComp );
	}

	if( TilePtr != nullptr )
	{
		(*TilePtr)->HexTileType = HexTile;
	}
	else
	{
		UHexTileData* NewTileData = NewObject<UHexTileData>(this);
		//There has to be a better way to do this
		NewTileData->HexLocation = Hex;
		NewTileData->HexTileType = HexTile;
		HexMap.Add( Hex, NewTileData );
	}
	//Update all of the instanced meshes to reflect the new changes.
	//When we add hexes one by one, we can update every addition. 
	//However when adding many hexes in a single frame it makes no sense to update every single time (we manually update in the end).
	if( CallUpdateMesh )
	{
		UpdateAllInstancedMeshes( );
	}
}

bool AHexMap::RemoveHex( const FHexVector& Hex, bool CallUpdateMesh /*= true */ )
{
	UHexTileData** Tile = HexMap.Find( Hex );
	if( Tile != nullptr )
	{
		HexMap.Remove( Hex );

		if( CallUpdateMesh )
		{
			UpdateAllInstancedMeshes();
		}

		return true;
	}
	else
	{
		return false;
	}


}

UHexTileData* AHexMap::GetDataAtHex( const FHexVector& Hex)
{
	UHexTileData** HexDataPtr = HexMap.Find( Hex );
	if( HexDataPtr != nullptr )
	{
		return ( *HexDataPtr );
	}
	else
	{
		return nullptr;
	}
}

bool AHexMap::RegisterActorToHex( const FHexVector& Hex, AActor* Actor )
{
	//TODO: If it turns out it won't be that costly, check to make sure actor is not registered anywhere else

	UHexTileData** HexDataPtr = HexMap.Find( Hex );
	if( HexDataPtr != nullptr && (* HexDataPtr) != nullptr )
	{
		(*HexDataPtr)->ActorsOnTile.Add( Actor );
		return true;
	}
	else
	{
		return false;
	}
}

bool AHexMap::UnregisterActorFromHex( const FHexVector& Hex, AActor* Actor )
{
	UHexTileData** HexDataPtr = HexMap.Find( Hex );
	if( HexDataPtr != nullptr && ( *HexDataPtr ) != nullptr )
	{
		( *HexDataPtr )->ActorsOnTile.Remove( Actor );
		return true;
	}
	else
	{
		return false;
	}
}

bool AHexMap::HasMeshBeenInstanced( const UStaticMesh* Mesh )
{
	bool MeshInstanced = false;
	for ( auto& Elem : InstancedMeshComponentMap )
	{
		UInstancedStaticMeshComponent* ISMComp = Elem.Value;
		if( ISMComp->GetStaticMesh() == Mesh )
		{
			MeshInstanced = true;
			break;
		}
	}
	return MeshInstanced;
}

void AHexMap::UpdateAllInstancedMeshes()
{
	for( auto& Elem : InstancedMeshComponentMap )
	{
		UInstancedStaticMeshComponent* ISMComp = Elem.Value;
		ISMComp->ClearInstances();
	}
	for( auto& Elem : HexMap )
	{
		UHexTileData* HexTileData = Elem.Value;
		UStaticMesh* TileMesh = ( Cast<UAbstractHexTile>( HexTileData->HexTileType->GetDefaultObject() ) )->TileMesh;
		UInstancedStaticMeshComponent** ISMCompPtr = InstancedMeshComponentMap.Find( TileMesh );
		if( ISMCompPtr != nullptr )
		{
			FTransform MeshTransform = FTransform( HexVectorToPoint( HexTileData->HexLocation ) );
			( *ISMCompPtr )->AddInstanceWorldSpace( MeshTransform );
		}
	}
}

TArray<FHexVector> AHexMap::GetPathBetweenHexes( FHexVector Start, FHexVector End )
{
	//I'm defining the struct here cause we don't need it anywhere else
	struct FHexVectorPathData
	{
		int32 F, G;
		FHexVector CameFrom;

		FHexVectorPathData(int32 F_, int32 G_, FHexVector CameFrom_)
			: F(F_), G(G_), CameFrom(CameFrom_)
		{}
	};

	TArray<FHexVector> PathArray = TArray<FHexVector>();
	TMap<FHexVector, FHexVectorPathData> PathDataMap = TMap<FHexVector, FHexVectorPathData>();
	TSet<FHexVector> OpenSet = TSet<FHexVector>();
	TSet<FHexVector> ClosedSet = TSet<FHexVector>();

	if( Start == End )
	{
		return PathArray;
	}
	else
	{
		OpenSet.Add( Start );
		PathDataMap.Add( Start, FHexVectorPathData( 0, FHexVector::DistanceBetween( Start, End ), Start ) );
		while( OpenSet.Num() > 0 )
		{
			//Get OpenSet Key with lowest F value
			TArray<FHexVector> OpenArray = OpenSet.Array();
			FHexVector LowestFKey = OpenArray[0];
			int32 LowestFValue = PathDataMap.FindChecked( OpenArray[0] ).F;
			for( int32 i = 1; i < OpenArray.Num(); i++ )
			{
				int32 FValue = PathDataMap.FindChecked( OpenArray[i] ).F;
				if( FValue < LowestFValue )
				{
					LowestFKey = OpenArray[i];
					LowestFValue = FValue;
				}
			}

			FHexVector Current = LowestFKey;

			//If we reached the end, reconstruct the path
			if( Current == End )
			{
				TArray<FHexVector> ReconstructedPath = TArray<FHexVector>();
				while( Current != Start )
				{
					ReconstructedPath.Insert( Current, 0 );
					Current = PathDataMap.FindChecked( Current ).CameFrom;
				}
				return ReconstructedPath;
			}
			else
			{
				OpenSet.Remove( Current );
				ClosedSet.Add( Current );
				TArray<FHexVector> Neighbors = FHexVector::GetNeighbors( Current );
				for( FHexVector& Neighbor : Neighbors )
				{
					UHexTileData** Tile = HexMap.Find( Neighbor );
					if( !ClosedSet.Contains( Neighbor ) 
						&& Tile != nullptr 
						&& !Cast<UAbstractHexTile>( (*Tile)->HexTileType->GetDefaultObject() )->IsWall )
					{
						int32 TentativeGScore = PathDataMap.FindChecked( Current ).G + FHexVector::DistanceBetween( Current, Neighbor );

						if( !OpenSet.Contains( Neighbor ) )
						{
							OpenSet.Add( Neighbor );
							PathDataMap.Add( Neighbor, FHexVectorPathData( TentativeGScore + FHexVector::DistanceBetween( Neighbor, End ), 
																		   TentativeGScore, 
												                           Current ) );
						}
						else if( TentativeGScore < PathDataMap.FindChecked( Neighbor ).G )
						{
							PathDataMap.FindChecked( Neighbor ).CameFrom = Current;
							PathDataMap.FindChecked( Neighbor ).G = TentativeGScore;
							PathDataMap.FindChecked( Neighbor ).F = TentativeGScore + FHexVector::DistanceBetween( Neighbor, End );
						}
					}
				}
			}
		}
		//Will be empty (essentially failure)
		return PathArray;
	}
}

void AHexMap::RecalculateLights()
{
	LitTiles.Empty();
	for( UHexLightComponent* LightSource : ActiveLights )
	{
		FHexVector LightSourcePos = LightSource->GetLightPosition();
		TArray<FHexVector> TilesToLightUp = UHexFunctionLibrary::HexInRadius( LightSourcePos, 
																			  LightSource->LightRadius);
		for( FHexVector& TileLight : TilesToLightUp )
		{
			bool WallHit = false;
			TArray<FHexVector> TilesToCheck = UHexFunctionLibrary::HexLine( LightSourcePos, TileLight );
			if( TilesToCheck.Num() > 0 )
			{
				for( FHexVector& TileCheck : TilesToCheck )
				{
					UAbstractHexTile* TileData = UHexFunctionLibrary::GetTileTypeObject( this, TileCheck );
					if( !TileData || TileData->IsWall )
					{
						WallHit = true;
						break;
					}
				}
			}
			if( !WallHit )
			{
				LitTiles.Add( TileLight );
			}
		}
	}
	NotifyTileActorsOfLightChange();
	
}

bool AHexMap::IsTileLitUp( FHexVector HexLocation )
{
	return LitTiles.Find( HexLocation ) != NULL;
}

void AHexMap::AddLightSource( UHexLightComponent* LightSource )
{
	ActiveLights.Add( LightSource );
	RecalculateLights();
}

void AHexMap::RemoveLightSource( UHexLightComponent* LightSource )
{
	ActiveLights.Remove( LightSource );
	RecalculateLights();
}
