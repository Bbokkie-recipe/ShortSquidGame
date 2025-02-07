// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/NetRaceGameState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/NetRacePlayerState.h"
#include "Doll/Doll.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/NetRaceGameMode.h"
#include "GameFramework/PlayerState.h"

TArray<class APlayerState*> ANetRaceGameState::GetMyPlayerList()
{
    PlayerArray.Sort(ANetRaceGameState::DescendingByRemainTime);
    // 이름순 정렬
    PlayerArray.Sort(ANetRaceGameState::AscendingByString);
    // 점수순 정렬
    PlayerArray.Sort([](const APlayerState& ps1, const APlayerState& ps2) {
        return ps1.GetScore() >= ps2.GetScore();
        });

    return PlayerArray;
}

bool ANetRaceGameState::DescendingByRemainTime(const APlayerState& ps1, const APlayerState& ps2)
{
    const ANetRacePlayerState* PlayerState1 = Cast<const ANetRacePlayerState>(&ps1);
    const ANetRacePlayerState* PlayerState2 = Cast<const ANetRacePlayerState>(&ps2);

    return PlayerState1->GetArrivalTime() >= PlayerState2->GetArrivalTime();
}


// 오름차순
bool ANetRaceGameState::AscendingByString(const APlayerState& ps1, const APlayerState& ps2)
{
    return ps1.GetPlayerName() <= ps2.GetPlayerName();
}

// 내림차순
bool ANetRaceGameState::DescendingByString(const APlayerState& ps1, const APlayerState& ps2)
{
    return ps1.GetPlayerName() >= ps2.GetPlayerName();
}

ANetRaceGameState::ANetRaceGameState()
{
    int32 PlayerCount = PlayerArray.Num();
	GameStartTime = 0;
	GameEndTime = 0;

	TimeLimitInSeconds = 180.0f; // 3min
    SquidGameState = EGamePlayState::WaitingToStart;
    SongState = ESongState::None;
	bReplicates = true;
}

void ANetRaceGameState::SingSong()
{
    SongState = ESongState::DollSong;
}

void ANetRaceGameState::SearchMoving()
{
    SongState = ESongState::SearchTime;
}


void ANetRaceGameState::StartGame()
{
    SquidGameState = EGamePlayState::InProgress;
    FString LogMessage = FString::Printf(TEXT("VariableName: %d"), PlayerArray.Num());
    FColor MessageColor = FColor::Green;
    float DisplayTime = 5.0f;
    //GEngine->AddOnScreenDebugMessage(-1, DisplayTime, MessageColor, LogMessage);
}

void ANetRaceGameState::EndGame()
{
    UE_LOG(LogTemp, Warning, TEXT("EndGame\n"));
    SquidGameState = EGamePlayState::GameOver;
}

void ANetRaceGameState::DoolSongState()
{
    UE_LOG(LogTemp, Warning, TEXT("DoolSongState\n"));
    SongState = ESongState::DollSong;
}

void ANetRaceGameState::DoolSearchState()
{
    UE_LOG(LogTemp, Warning, TEXT("DoolSearchState\n"));
    SongState = ESongState::SearchTime;
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

void ANetRaceGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetRaceGameState, SquidGameState);
    DOREPLIFETIME(ANetRaceGameState, SongState);
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
        if (HasAuthority()) {

            serverDollSongStart(FoundDoll);
        }
    }
    return AllPlayersReady;
}

void ANetRaceGameState::serverDollSongStart_Implementation(class ADoll* _FoundDoll)
{
    UE_LOG(LogTemp, Warning, TEXT("_FoundDoll serverDollSongStart_Implementation"));
    MulticastDollSongStart(_FoundDoll);
}

void ANetRaceGameState::MulticastDollSongStart_Implementation(class ADoll* _FoundDoll)
{
    //wolrd()->aoll->Audio();
    
    FTimerHandle createHandler;
    if (_FoundDoll) {
        UE_LOG(LogTemp, Warning, TEXT("_FoundDoll sMulticastDollSongStart_Implementation"));
        GetWorld()->GetTimerManager().SetTimer(createHandler, FTimerDelegate::CreateLambda([&]() {
            Cast<ADoll>(UGameplayStatics::GetActorOfClass(GetWorld(), ADoll::StaticClass()))->StartDoolAudio();
            }), 1.0f, false);
    }
}

void ANetRaceGameState::SearchDoll()
{
    FoundDoll = Cast<ADoll>(UGameplayStatics::GetActorOfClass(GetWorld(), ADoll::StaticClass()));
}


bool ANetRaceGameState::EndSquidPlay()
{
    bool AllPlayersDone = false;
    TArray<TObjectPtr<APlayerState>> Players = this->PlayerArray;
    if (Players.Num() == 1){ // Single Play
        ANetRacePlayerState* PS = Cast<ANetRacePlayerState>(Players[0]);
        if (PS) {
            if (PS->GetPassed() || PS->GetisDead()) {
                AllPlayersDone = true;
            }
        }
    }
    else {
        AllPlayersDone = true; 
        for (APlayerState* PlayerState : Players) {
            ANetRacePlayerState* PS = Cast<ANetRacePlayerState>(PlayerState);
            if (PS) {
                if (!PS->GetPassed() && !PS->GetisDead()) {
                    AllPlayersDone = false;
                    break;
                }
            }
        }
    }
    if (SquidGameState == EGamePlayState::GameOver) {
        return true;
    }

    return AllPlayersDone;
}


FString ANetRaceGameState::GetGameStateAsString()
{
    FString nowGS = "";
    if (SongState == ESongState::DollSong) nowGS.Append("DollSong");
    if (SongState == ESongState::None) nowGS.Append("DollNone");
    if (SongState == ESongState::SearchTime) nowGS.Append("DollSearch");
    if (EndSquidPlay())nowGS.Append("Squid Game End");
    if (SquidGameState == EGamePlayState::WaitingToStart) return FString(nowGS.Append("WaitingToStart"));
    if (SquidGameState == EGamePlayState::InProgress) return FString(nowGS.Append("InProgress"));
    if (SquidGameState == EGamePlayState::GameOver) return FString(nowGS.Append("GameOver"));
    return FString("UnknownGameState");
}