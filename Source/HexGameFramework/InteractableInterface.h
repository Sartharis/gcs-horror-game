#pragma once

#include "InteractableInterface.generated.h"

UINTERFACE( BlueprintType )
class HEXGAMEFRAMEWORK_API UInteractableInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class HEXGAMEFRAMEWORK_API IInteractableInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable, Category = "MyCategory" )
	bool CanBeInteractedWith();

	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable, Category = "MyCategory" )
	void OnInteraction();

};