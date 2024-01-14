// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/NetRaceGameState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/NetRacePlayerState.h"

ANetRaceGameState::ANetRaceGameState()
{
    int32 PlayerCount = PlayerArray.Num();
	GameStartTime = 0;
	GameEndTime = 0;

	TimeLimitInSeconds = 180.0f; // 3min
	GameState = EGamePlayState::WaitingToStart;

	bReplicates = true;
}

void ANetRaceGameState::StartGame()
{
    GameState = EGamePlayState::InProgress;
}

void ANetRaceGameState::EndGame()
{
    GameState = EGamePlayState::GameOver;
}

int32 ANetRaceGameState::GetElapsedGameTime() const
{
    /*
    int32 CurrentTime = FDateTime::Now().ToUnixTimestamp();
    int32 ElapsedTime = CurrentTime - GameStartTime;

    ElapsedTime = FMath::Max(0, ElapsedTime);
    */
    return ElapsedTime;
}

void ANetRaceGameState::DefaultTimer()
{
    Super::DefaultTimer();

    if (IsMatchInProgress())
    {
        ++ElapsedTime;

        if (IsGameOverCondition())
        {
            EndGame();
        }
    }
}

bool ANetRaceGameState::IsGameOverCondition() const
{
    bool AllPlayersDead = true;
    for (APlayerState* PlayerState : PlayerArray)
    {
        ANetRacePlayerState* NetRacePlayer = Cast<ANetRacePlayerState>(PlayerState); 
        if (NetRacePlayer && NetRacePlayer->GetisDead())
        {
            AllPlayersDead = false;
            break;
        }
    }

    bool TimeExpired = (ElapsedTime >= 180);

    bool AllCrossedFinish = true;
    for (APlayerState* PlayerState : PlayerArray)
    {
        ANetRacePlayerState* NetRacePlayer = Cast<ANetRacePlayerState>(PlayerState);
        if (NetRacePlayer && !NetRacePlayer->GetHasCrossedFinish())
        {
            AllCrossedFinish = false;
            break;
        }
    }

    return AllPlayersDead || TimeExpired || AllCrossedFinish;
}


void ANetRaceGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetRaceGameState, GameState);
	DOREPLIFETIME(ANetRaceGameState, GameStartTime);
}