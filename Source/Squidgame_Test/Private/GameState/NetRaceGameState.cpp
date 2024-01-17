// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/NetRaceGameState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/NetRacePlayerState.h"
#include "Doll/Doll.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/NetRaceGameMode.h"
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
    FString LogMessage = FString::Printf(TEXT("VariableName: %d"), PlayerArray.Num());
    FColor MessageColor = FColor::Green;
    float DisplayTime = 5.0f;
    //GEngine->AddOnScreenDebugMessage(-1, DisplayTime, MessageColor, LogMessage);
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
    if (FoundDoll == nullptr) {
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
        for (APlayerState* PlayerState : Players) {
            //UE_LOG(LogTemp, Warning, TEXT("APlayerState* PlayerState : Players"));
            ANetRaceGameMode* MyGameMode = Cast<ANetRaceGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
            if (MyGameMode)
            {
                //UE_LOG(LogTemp, Warning, TEXT("MyGameMode->StartRaceGame()"));
                MyGameMode->StartRaceGame();
            }
        }

        FTimerHandle createHandler;
        GetWorld()->GetTimerManager().SetTimer(createHandler, FTimerDelegate::CreateLambda([&]() {
            FoundDoll->StartDoolAudio();
            }), 1.0f, false);
    }
    return AllPlayersReady;
}

void ANetRaceGameState::SearchDoll()
{
    if (HasAuthority()) {
        FoundDoll = Cast<ADoll>(UGameplayStatics::GetActorOfClass(GetWorld(), ADoll::StaticClass()));
    }
}
