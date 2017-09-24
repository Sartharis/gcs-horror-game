// Fill out your copyright notice in the Description page of Project Settings.

#include "HexGameFramework.h"
#include "HexFunctionLibrary.h"
#include "HexGameFrameworkGameModeBase.h"
#include "TurnManager.h"

UTurnManager::UTurnManager()
	: TurnOrder{ ETurnCategory::TC_PLAYER, ETurnCategory::TC_ENEMY, ETurnCategory::TC_NEUTRAL }
	, CurrentTurn( 0 ), CurrentRound( 0 )
{
}

void UTurnManager::RegisterTurnComponent( UTurnComponent* TurnComponent, bool InsertIntoTurn /*= false */ )
{
	if( TurnComponentsRegistered.Find( TurnComponent ) != INDEX_NONE )
	{
		
	}
	else
	{
		TurnComponentsRegistered.Add( TurnComponent );
		if( InsertIntoTurn && TurnComponent->TurnType == CurrentTurn )
		{
			TurnComponentsLeftToMove.AddUnique( TurnComponent );
			TurnComponent->StartTurn();
		}
		OnTurnStateUpdate();
	}
}

void UTurnManager::UnregisterTurnComponent( UTurnComponent* TurnComponent )
{
	if( TurnComponentsRegistered.Find( TurnComponent ) != INDEX_NONE )
	{
		FinishMove( TurnComponent );
		TurnComponentsRegistered.Remove( TurnComponent );
	}
	else
	{

	}
}

void UTurnManager::FinishMove( UTurnComponent* TurnComponent )
{
	if( TurnComponentsLeftToMove.Find( TurnComponent ) != INDEX_NONE )
	{
		TurnComponentsLeftToMove.Remove( TurnComponent );
		OnTurnStateUpdate();
	}
}

void UTurnManager::OnTurnStateUpdate()
{
	if( TurnComponentsLeftToMove.Num() <= 0 )
	{
		StartNextTurn();
	}
}

void UTurnManager::ResetTurnManager()
{
	CurrentTurn = 0;
	CurrentRound = 0;
	TurnComponentsLeftToMove.Empty();
	ActivateTurnComponents();
}

void UTurnManager::StartNextTurn()
{
	CurrentTurn += 1;
	//If current turn is equal to the turn order array, then we finished the round
	if( CurrentTurn == TurnOrder.Num() )
	{
		CurrentTurn = 0;
		StartNextRound();
	}

	ActivateTurnComponents();
}

void UTurnManager::ActivateTurnComponents()
{
	if( TurnComponentsRegistered.Num() > 0 )
	{
		for( UTurnComponent* TurnComponent : TurnComponentsRegistered )
		{
			if( TurnComponent->TurnType == TurnOrder[CurrentTurn] )
			{
				TurnComponentsLeftToMove.AddUnique( TurnComponent );
				TurnComponent->StartTurn();
			}
		}
	}
	OnTurnStateUpdate();
}

void UTurnManager::StartNextRound()
{
	CurrentRound += 1;
}

//TURN COMPONENT-------------------------------------------------------------------------------------
UTurnComponent::UTurnComponent()
	: Active( false )
{

}

bool UTurnComponent::IsInTurn()
{
	return Active;
}

void UTurnComponent::RemoveFromTurnManager()
{
	UTurnManager* TurnManager = UHexFunctionLibrary::GetHexGameMode( this )->GetTurnManager();
	check( TurnManager != nullptr );
	TurnManager->UnregisterTurnComponent( this );
}

void UTurnComponent::AddToTurnManager( bool InsertIntoTurn)
{
	UTurnManager* TurnManager = UHexFunctionLibrary::GetHexGameMode( this )->GetTurnManager();
	check( TurnManager != nullptr );
	TurnManager->RegisterTurnComponent( this, InsertIntoTurn );
}

void UTurnComponent::StartTurn()
{
	Active = true;
	TurnActivateDelegate.Broadcast(); 
}

void UTurnComponent::EndTurn()
{
	Active = false;
	TurnEndDelegate.Broadcast();

	UTurnManager* TurnManager = UHexFunctionLibrary::GetHexGameMode( this )->GetTurnManager();
	check( TurnManager != nullptr );
	TurnManager->FinishMove( this );
}

void UTurnComponent::BeginPlay()
{
	Super::BeginPlay();
	AddToTurnManager( true );
}
