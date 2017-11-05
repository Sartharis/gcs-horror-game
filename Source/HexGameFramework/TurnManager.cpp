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

int32 UTurnManager::GetCurrentRound()
{
	return CurrentRound;
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
			ActivateTurnComponent( TurnComponent );
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
	else if(TurnComponentsLeftToMoveNonSync.Find(TurnComponent) != INDEX_NONE)
	{
		TurnComponentsLeftToMoveNonSync.Remove( TurnComponent );
		OnTurnStateUpdate();
	}
}

bool UTurnManager::FinishTurn()
{
	if( TurnBlockers.Num() <= 0 )
	{
		TurnComponentsLeftToMove.Empty();
		TurnComponentsLeftToMoveNonSync.Empty();
		OnTurnStateUpdate();
		return true;
	}
	return false;
}

ETurnCategory::Type UTurnManager::GetCurrentTurnCategory()
{
	return TurnOrder[CurrentTurn];
}

void UTurnManager::OnTurnStateUpdate()
{
	if( TurnComponentsLeftToMove.Num() <= 0 && CountRegisteredTypes() >= 2 )
	{
		if( TurnComponentsLeftToMoveNonSync.Num() > 0 )
		{
			int32 MaxNum = TurnComponentsLeftToMoveNonSync.Num();
			TurnComponentsLeftToMoveNonSync[FMath::RandRange(0,MaxNum - 1)]->StartTurn();
		}
		else
		{
			StartNextTurn();
		}
	}
}

void UTurnManager::ResetTurnManager()
{
	CurrentTurn = 0;
	CurrentRound = 0;
	TurnComponentsLeftToMove.Empty();
	TurnComponentsLeftToMoveNonSync.Empty();
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
				ActivateTurnComponent( TurnComponent );
			}
		}
	}
	OnTurnStateUpdate();
}

void UTurnManager::ActivateTurnComponent( UTurnComponent* TurnComponent )
{
	if( TurnComponent->Synchronous )
	{
		TurnComponent->StartTurn();
		TurnComponentsLeftToMove.AddUnique( TurnComponent );
	}
	else
	{
		TurnComponentsLeftToMoveNonSync.AddUnique( TurnComponent );
	}
}

void UTurnManager::StartNextRound()
{
	CurrentRound += 1;
	RoundStart.Broadcast(CurrentRound);
}

int UTurnManager::CountRegisteredTypes()
{
	TArray<int32> Existingtypes;
	if( TurnComponentsRegistered.Num() > 0 )
	{
		for( UTurnComponent* TurnComponent : TurnComponentsRegistered )
		{
			if( Existingtypes.Find(TurnComponent->TurnType) == INDEX_NONE)
			{
				Existingtypes.Add( TurnComponent->TurnType );
			}
		}
	}
	return Existingtypes.Num();
}

bool UTurnManager::AddTurnBlocker( FString BlockerName )
{
	if( TurnBlockers.Find( BlockerName ) != NULL )
	{
		return false;
	}
	else
	{
		TurnBlockers.Add( BlockerName );
		return true;
	}
}

bool UTurnManager::RemoveTurnBlocker( FString BlockerName )
{
	if( TurnBlockers.Find( BlockerName ) != NULL )
	{
		TurnBlockers.Remove( BlockerName );
		return true;
	}
	else
	{
		return false;
	}
}

//TURN COMPONENT-------------------------------------------------------------------------------------
UTurnComponent::UTurnComponent()
	: Active( false )
{
	PrimaryComponentTick.bCanEverTick = false;
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

void UTurnComponent::BlockTurnEnd()
{
	UTurnManager* TurnManager = UHexFunctionLibrary::GetHexGameMode( this )->GetTurnManager();
	check( TurnManager != nullptr );
	TurnManager->AddTurnBlocker( this->GetName() );
}

void UTurnComponent::UnblockTurnEnd()
{
	UTurnManager* TurnManager = UHexFunctionLibrary::GetHexGameMode( this )->GetTurnManager();
	check( TurnManager != nullptr );
	TurnManager->RemoveTurnBlocker( this->GetName() );
}

void UTurnComponent::BeginPlay()
{
	Super::BeginPlay();
	AddToTurnManager( true );
}
