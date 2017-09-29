#pragma once

#include "Hex.h"
#include "Components/ActorComponent.h"
#include "HexLightComponent.generated.h"

/**
*
*/
UCLASS( ClassGroup = ( Common ), BlueprintType, meta = ( BlueprintSpawnableComponent ) )
class HEXGAMEFRAMEWORK_API UHexLightComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHexLightComponent();

	/** How far the light source shines*/
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Light" )
	int LightRadius;

	/** Should light be on when game begins?*/
	UPROPERTY( BlueprintReadOnly, EditAnywhere, Category = "Light" )
	bool StartActive;
	
	UFUNCTION( BlueprintCallable, Category = "Light" )
	void SetLightPosition( FHexVector NewPosition );
	
	UFUNCTION( BlueprintPure, Category = "Light" )
	FHexVector GetLightPosition();

	/** Toggle the light on or off*/
	UFUNCTION( BlueprintCallable, Category = "Light" )
	void ToggleLight( bool Activate );
	
	/** Check whether the light is currently active */
	UFUNCTION( BlueprintPure, Category = "Light")
	bool LightActive();

	virtual void BeginPlay() override;

private:

	/** Where on the map is the light source*/
	UPROPERTY()
	FHexVector LightLocation;

	/** Is the light currently on or off?*/
	UPROPERTY()
	bool Active;
};
