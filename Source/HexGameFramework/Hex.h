#pragma once

#include <functional>
#include "HexGameFramework.h"
#include "Hex.generated.h"

// Hex coordinates are stored in cube coordinates for simplifying math operations. Think of this as a vector for hex coordinates.
USTRUCT( BlueprintType )
struct FHexVector
{
	GENERATED_USTRUCT_BODY()

	/** The first of the three cube coordinates for our hex cube coords */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Hex Cube Coordinates" )
	int32 Q;

	/** The second of the three cube coordinates for our hex cube coords */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Hex Cube Coordinates" )
	int32 R; 

	/** The third of the three cube coordinates for our hex cube coords */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Hex Cube Coordinates" )
	int32 S;

	/** Default constructor (no initialization). */
	FORCEINLINE FHexVector() { }

	//Constructor for Axial coordinates (Converts to cube)
	FORCEINLINE FHexVector( int32 Q_, int32 R_ );

	//Constructor for Cube coordinates
	FORCEINLINE FHexVector( int32 Q_, int32 R_, int32 S_ );

public:

	//We define +, -, * (and so on ... ) operators for FHexCoords to make future functions simpler.
	//If you need handling for another operator, just add it here.

	FORCEINLINE FHexVector operator+( const FHexVector& H ) const;

	FORCEINLINE FHexVector operator-( const FHexVector& H ) const;

	FORCEINLINE FHexVector operator*( float Scale ) const;

	bool operator== ( const FHexVector& H ) const;

	bool operator!= ( const FHexVector& H ) const;

	// Other useful Hex functions
	/** Distance of Hex coordinate from 0,0,0*/
	FORCEINLINE int Length() const;

	/** Distance between two hex vectors*/
	FORCEINLINE static int DistanceBetween( const FHexVector& A, const FHexVector& B );

	/** Get a hex vector for a given direction */
	FORCEINLINE static FHexVector GetHexDirection( int direction );

	/** Get an array containing all neighbors of the given hex vector*/
	FORCEINLINE static TArray<FHexVector> GetNeighbors( const FHexVector& H );

	/** Get the coordinates of the neighbor of the given hex in the given direction */
	FORCEINLINE static FHexVector GetNeighborCoords( const FHexVector& H, int direction );
	
	FORCEINLINE friend uint32 GetTypeHash( const FHexVector& Hex );

};

FORCEINLINE FHexVector::FHexVector( int32 Q_, int32 R_ )
	: Q( Q_ ), R( R_ ), S( -Q_ - R_ ) 
{}

FORCEINLINE FHexVector::FHexVector( int32 Q_, int32 R_, int32 S_ )
	: Q( Q_ ), R( R_ ), S( S_ ) 
{}

FORCEINLINE FHexVector FHexVector::operator+( const FHexVector& H ) const
{
	return FHexVector( Q + H.Q, R + H.R, S + H.S );
}

FORCEINLINE FHexVector FHexVector::operator-( const FHexVector& H ) const
{
	return FHexVector( Q - H.Q, R - H.R, S - H.S );
}

FORCEINLINE FHexVector FHexVector::operator*( float Scale ) const
{
	return FHexVector( Q * Scale, R * Scale, S * Scale );
}

FORCEINLINE bool FHexVector::operator==( const FHexVector& H ) const
{
	return Q == H.Q && R == H.R && S == H.S;
}

FORCEINLINE bool FHexVector::operator!=( const FHexVector& H ) const
{
	return Q != H.Q || R != H.R || S != H.S;
}

FORCEINLINE int FHexVector::Length() const
{
	return int( ( FMath::Abs( Q ) + FMath::Abs( R ) + FMath::Abs( S ) ) / 2 );
}

FORCEINLINE int FHexVector::DistanceBetween( const FHexVector& A, const FHexVector& B )
{
	return (A - B).Length();
}

FORCEINLINE FHexVector FHexVector::GetHexDirection( int direction )
{
	check( 0 <= direction && direction < 6 );
	switch( direction )
	{
		case 0:	return FHexVector( 1, 0, -1 );
		case 1: return FHexVector( 1, -1, 0 );
		case 2:	return FHexVector( 0, -1, 1 );
		case 3: return FHexVector( -1, 0, 1 );
		case 4:	return FHexVector( -1, 1, 0 );
		case 5:	return FHexVector( 0, 1, -1 );
		default: return FHexVector( 1, 0, -1 );
	}
}

FORCEINLINE TArray<FHexVector> FHexVector::GetNeighbors( const FHexVector& H )
{
	TArray<FHexVector> Neighbors = TArray<FHexVector>();
	for( int32 i = 0; i < 6; i++ )
	{
		Neighbors.Add( GetNeighborCoords( H, i ) );
	}
	return Neighbors;
}

FORCEINLINE FHexVector FHexVector::GetNeighborCoords( const FHexVector& H, int direction )
{
	return (H + GetHexDirection( direction ));
}

FORCEINLINE uint32 GetTypeHash( const FHexVector& Hex )
{
	std::hash<int> int_hash;
	size_t hq = int_hash( Hex.Q );
	size_t hr = int_hash( Hex.R );
	return hq ^ ( hr + 0x9e3779b9 + ( hq << 6 ) + ( hq >> 2 ) );
}
