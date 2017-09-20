// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGameFramework.h"
#include "HexTile.h"

UAbstractHexTile::UAbstractHexTile()
{
}

ADropTest::ADropTest()
{
	#if WITH_EDITORONLY_DATA
		USelection::SelectionChangedEvent.AddUObject( this, &ADropTest::OnObjectSelected );
	#endif
}

#if WITH_EDITORONLY_DATA
void ADropTest::OnObjectSelected( UObject* Object )
{
	if( Object == this )
	{
		
	}
	else if( !IsSelected() )
	{
		Destroy();
	}
}
#endif