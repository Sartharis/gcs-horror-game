// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TurnManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FTurnActivate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FTurnEnd );

UENUM( BlueprintType )
namespace ETurnCategory
{
	enum Type
	{
		TC_PLAYER UMETA( DisplayName = "Player" ),
		TC_ENEMY UMETA( DisplayName = "Enemy" ),
		TC_NEUTRAL UMETA( DisplayName = "Neutral")
	};
}

/**
*
*/
UCLASS( ClassGroup = ( Common ), BlueprintType, meta = ( BlueprintSpawnableComponent ) )
class HEXGAMEFRAMEWORK_API UTurnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTurnComponent();

	/** Sends a broadcast when the component starts its turn*/
	UPROPERTY( BlueprintAssignable, Category = "Turn" )
	FTurnActivate TurnActivateDelegate;

	/** Sends a broadcast when the component ends its turn*/
	UPROPERTY( BlueprintAssignable, Category = "Turn" )
	FTurnEnd TurnEndDelegate;

	/**Determines when the component will be given a turn to move*/
	UPROPERTY( EditAnywhere, Category = "Turn" )
	TEnumAsByte<ETurnCategory::Type> TurnType;

	/** Returns true when component has not finished its turn and can still take actions*/
	UFUNCTION( BlueprintPure, Category="Turn")
	bool IsInTurn();

	/** Only called by the turn manager, starts the component's turn*/
	void StartTurn();

	/** Ends the component's turn */
	UFUNCTION( BlueprintCallable, Category="Turn" )
	void EndTurn();

	virtual void BeginPlay() override;

private:

	/** true iff the component can still take an action */
	UPROPERTY()
	bool Active;
};

/**
 * 
 */
UCLASS()
class HEXGAMEFRAMEWORK_API UTurnManager : public UObject
{
	GENERATED_BODY()
public:
	UTurnManager();
	
	void RegisterTurnComponent( UTurnComponent* TurnComponent, bool InsertIntoTurn = false );
	void UnregisterTurnComponent( UTurnComponent* TurnComponent );

	/**Called by turn components to signify they finished their turn*/
	void FinishMove( UTurnComponent* TurnComponent );
	void ResetTurnManager();

private:

	TArray<UTurnComponent*> TurnComponentsRegistered;
	TArray<ETurnCategory::Type> TurnOrder;
	TArray<UTurnComponent*> TurnComponentsLeftToMove;
	int32 CurrentTurn;
	int32 CurrentRound;

	/** Called whenever some turn component changes its turn state (finishes, starts, registers etc.)*/
	void OnTurnStateUpdate();

	void StartNextTurn();
	void ActivateTurnComponents();
	void StartNextRound();
};
