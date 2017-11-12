// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGameFramework.h"
#include "HexMap.h"
#include "HexFunctionLibrary.h"
#include "TileObject.h"


// Sets default values
ATileObject::ATileObject()
{

}

void ATileObject::SetBlocking( bool Blocking )
{
	if( BlockActors != Blocking )
	{
		BlockActors = Blocking;
		UHexFunctionLibrary::GetHexGameMode( this )->WorldCollisionChange();
	}
}


