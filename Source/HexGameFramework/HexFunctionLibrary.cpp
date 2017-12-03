// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGameFramework.h"
#include "Hex.h"
#include "Public/UObject/Object.h"
#include "HexFunctionLibrary.h"

FHexVector UHexFunctionLibrary::HexRound( float Q, float R, float S )
{
	int QRound = int( FMath::RoundToInt( Q ) );
	int RRound = int( FMath::RoundToInt( R ) );
	int SRound = int( FMath::RoundToInt( S ) );
	float QDiff = FMath::Abs( QRound - Q );
	float RDiff = FMath::Abs( RRound - R );
	float SDiff = FMath::Abs( SRound - S );
	if( QDiff > RDiff && QDiff > SDiff )
	{
		QRound = -RRound - SRound;
	}
	else if( RDiff > SDiff )
	{
		RRound = -QRound - SRound;
	}
	else
	{
		SRound = -QRound - RRound;
	}
	return FHexVector( QRound, RRound, SRound );
}

TArray<FHexVector> UHexFunctionLibrary::HexLine( FHexVector A, FHexVector B )
{
	int Dist = HDist( A, B );
	TArray< FHexVector > Results;
	if( A == B )
	{
		Results.Add( A );
		return Results;
	}
	// We want to sample n+1 points
	for( int i = 0; i <= Dist; i++ )
	{
		float LerpVal = ( float(i) / float(Dist) );
		Results.Add( HexRound( FMath::Lerp( A.Q, B.Q, LerpVal )
							   , FMath::Lerp( A.R, B.R, LerpVal )
							   , FMath::Lerp( A.S, B.S, LerpVal ) ) );
	}
	return Results;
}

TArray<FHexVector> UHexFunctionLibrary::HexRing( FHexVector Center, int Radius )
{
	TArray< FHexVector > Results;
	if( Radius > 0 )
	{
		FHexVector CurrentHex = Center + ( FHexVector::GetHexDirection( 4 ) * Radius );
		for( int i = 0; i < 6; i++ )
		{
			for( int j = 0; j < Radius; j++ )
			{
				Results.Add( CurrentHex );
				CurrentHex = FHexVector::GetNeighborCoords( CurrentHex, i );
			}
		}
	}
	else
	{
		Results.Add( Center );
	}
	return Results;
}

UAbstractHexTile* UHexFunctionLibrary::GetTileTypeObject( AHexMap* HexMap, FHexVector Tile )
{
	if( HexMap && HexMap->GetDataAtHex( Tile ))
	{
		UAbstractHexTile* TileObj = Cast<UAbstractHexTile>( HexMap->GetDataAtHex( Tile )->HexTileType->GetDefaultObject() );
		return TileObj;
	}
	else
	{
		return NULL;
	}
}

TArray<FHexVector> UHexFunctionLibrary::HexInRadius( FHexVector Center, int Radius )
{
	TArray< FHexVector > Results;
	Results.Add( Center );
	if( Radius > 0 )
	{
		for ( int i = 1; i <= Radius; i++ )
		{
			TArray< FHexVector > HexToAdd = HexRing( Center, i );
			Results.Append( HexToAdd );	
		}
	}
	return Results;
}

bool UHexFunctionLibrary::Equal_HexVectorHexVector( FHexVector A, FHexVector B )
{
	return A == B;
}

FHexVector UHexFunctionLibrary::Add_HexVectorHexVector( FHexVector A, FHexVector B )
{
	return A + B;
}

FHexVector UHexFunctionLibrary::Subtract_HexVectorHexVector( FHexVector A, FHexVector B )
{
	return A - B;
}

FHexVector UHexFunctionLibrary::Multiply_HexVectorFloat( FHexVector A, float B )
{
	return A * B;
}

int UHexFunctionLibrary::HSize( FHexVector A )
{
	return A.Length();
}

int UHexFunctionLibrary::HDist( FHexVector A, FHexVector B )
{
	return FHexVector::DistanceBetween( A, B );
}

FHexVector UHexFunctionLibrary::MakeHexVectorAxial( int32 X, int32 Y )
{
	return FHexVector( X, Y );
}

AHexGameFrameworkGameModeBase* UHexFunctionLibrary::GetHexGameMode( const UObject* WorldContextObject )
{
	return Cast<AHexGameFrameworkGameModeBase>( UGameplayStatics::GetGameMode( WorldContextObject ) );
}

FVector2D UHexFunctionLibrary::GetGameViewportSize()
{
	FVector2D Result = FVector2D( 1, 1 );

	if( GEngine && GEngine->GameViewport )
	{
		GEngine->GameViewport->GetViewportSize( /*out*/Result );
	}

	return Result;
}

FVector2D UHexFunctionLibrary::GetGameResolution()
{
	FVector2D Result = FVector2D( 1, 1 );

	Result.X = GSystemResolution.ResX;
	Result.Y = GSystemResolution.ResY;

	return Result;
}

