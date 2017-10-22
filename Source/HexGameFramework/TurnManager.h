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

	/** true iff the component can still take an action in whatever turn is happening*/
	UPROPERTY( BlueprintReadOnly, Category = "Turn" )
	bool Active;

	/** Returns true when component has not finished its turn and can still take actions*/
	UFUNCTION( BlueprintPure, Category="Turn")
	bool IsInTurn();

	/**Remove the component from the turn manger if it is in the turn manager*/
	UFUNCTION( BlueprintCallable, Category = "Turn" )
	void RemoveFromTurnManager();

	/**Add the component to the turn manager if it not in the turn manager*/
	UFUNCTION( BlueprintCallable, Category = "Turn" )
	void AddToTurnManager( bool InsertIntoTurn );

	/** Only called by the turn manager, starts the component's turn*/
	void StartTurn();

	/** Ends the component's turn */
	UFUNCTION( BlueprintCallable, Category="Turn" )
	void EndTurn();

	virtual void BeginPlay() override;

	
};

/**
 * 
 */
UCLASS( BlueprintType )
class HEXGAMEFRAMEWORK_API UTurnManager : public UObject
{
	GENERATED_BODY()
public:
	UTurnManager();
	
	/** Add a turn component to track */
	void RegisterTurnComponent( UTurnComponent* TurnComponent, bool InsertIntoTurn = false );
	/** Remove a tracked turn component */
	void UnregisterTurnComponent( UTurnComponent* TurnComponent );

	/**Called by turn components to signify they finished their turn*/
	void FinishMove( UTurnComponent* TurnComponent );
	/**Forces The current turn to finish*/
	UFUNCTION(BlueprintCallable, Category="Turn")
	void FinishTurn();

	UFUNCTION(BlueprintPure, Category="Turn")
	ETurnCategory::Type GetCurrentTurnCategory();
	/**Reset the turn manager (like we were starting turns from scratch)*/
	UFUNCTION( BlueprintCallable, Category = "Turn" )
	void ResetTurnManager();

private:
	/** Turn components currently registered in the manager*/
	TArray<UTurnComponent*> TurnComponentsRegistered;
	/** The order the turns happen in (when this array ends we have a new round)*/
	TArray<ETurnCategory::Type> TurnOrder;
	/** How many components still have to move in this turn */
	TArray<UTurnComponent*> TurnComponentsLeftToMove;
	/** The current turn we are in (according to turn order). Resets every round*/
	int32 CurrentTurn;
	/** The current round we are in*/
	int32 CurrentRound;

	/** Called whenever some turn component changes its turn state (finishes, starts, registers etc.)*/
	void OnTurnStateUpdate();

	/** Move to the next turn in the turn order and activate components in that turn*/
	void StartNextTurn();
	/** Activate all turn components in the current turn*/
	void ActivateTurnComponents();
	/** Start the next round and reset the turn order array*/
	void StartNextRound();
};
