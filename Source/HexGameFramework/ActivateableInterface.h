#pragma once

#include "ActivateableInterface.generated.h"

UINTERFACE( BlueprintType )
class HEXGAMEFRAMEWORK_API UActivatableInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class HEXGAMEFRAMEWORK_API IActivatableInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable, Category = "MyCategory" )
	void OnActivate();

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable, Category = "MyCategory" )
	void OnDeactivate();

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable, Category = "MyCategory" )
	bool CanBeActivated();

};