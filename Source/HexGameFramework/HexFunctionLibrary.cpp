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

