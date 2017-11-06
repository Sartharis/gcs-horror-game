#include "HexGameFramework.h"
#include "HexMap.h"
#include "HexFunctionLibrary.h"
#include "HexGameFrameworkGameModeBase.h"
#include "HexLightComponent.h"

UHexLightComponent::UHexLightComponent()
{

}

void UHexLightComponent::SetLightPosition( FHexVector NewPosition )
{
	LightLocation = NewPosition;
	AHexMap* HexMap = UHexFunctionLibrary::GetHexGameMode( this )->CurrentHexMap;
	if( Active && HexMap != NULL )
	{
		HexMap->RecalculateLights();
	}
}

void UHexLightComponent::SetLightRadius( int NewRadius )
{
	LightRadius = NewRadius;
	AHexMap* HexMap = UHexFunctionLibrary::GetHexGameMode( this )->CurrentHexMap;
	if( Active && HexMap != NULL )
	{
		HexMap->RecalculateLights();
	}
}

FHexVector UHexLightComponent::GetLightPosition()
{
	return LightLocation;
}

void UHexLightComponent::ToggleLight( bool Activate )
{
	AHexMap* HexMap = UHexFunctionLibrary::GetHexGameMode( this )->CurrentHexMap;
	if( Activate != Active && HexMap != NULL)
	{
		if( Activate )
		{
			HexMap->AddLightSource( this );
		}
		else
		{
			HexMap->RemoveLightSource( this );
		}
		Active = Activate;
		OnLightChanged.Broadcast( Active );
	}
}

bool UHexLightComponent::LightActive()
{
	return Active;
}

void UHexLightComponent::BeginPlay()
{
	Super::BeginPlay();
	AHexMap* HexMap = UHexFunctionLibrary::GetHexGameMode( this )->CurrentHexMap;
	if( HexMap )
	{
		SetLightPosition( HexMap->PointToHexVector( GetOwner()->GetActorLocation() ) );
	}
	if( StartActive)
	{
		ToggleLight( true );
	}
}

