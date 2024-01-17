// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/NetRaceGameState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/NetRacePlayerState.h"
#include "Doll/Doll.h"
#include "Kismet/GameplayStatics.h"

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
    DOREPLIFETIME(ANetRaceGameState, GameEndTime);
}

bool ANetRaceGameState::ReadyPlay()
{
    if (DollActor == nullptr) {
        SearchDoll();
    }
    bool AllPlayersReady = false;
    TArray<TObjectPtr<APlayerState>> Players = this->PlayerArray;
    if (Players.Num() == 1) { // Single Play
        AllPlayersReady = true;
    }
    else {
        for (APlayerState* PlayerState : Players) {
            ANetRacePlayerState* PS = Cast<ANetRacePlayerState>(PlayerState);
            if (PS && !(PS->bIsReady))
            {
                AllPlayersReady = false;
                break;
            }
            AllPlayersReady = true;
        }
    }

    if (AllPlayersReady)
    {
        FTimerHandle createHandler;
        GetWorld()->GetTimerManager().SetTimer(createHandler, FTimerDelegate::CreateLambda([&]() {
            DollActor->StartDoolAudio();
            }), 2.0f, false);
    }
    return AllPlayersReady;
}

void ANetRaceGameState::SearchDoll()
{
    Super::BeginPlay();
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoll::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        DollActor = Cast<ADoll>(FoundActors[0]);
    }
}
