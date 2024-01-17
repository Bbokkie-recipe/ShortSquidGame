// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NetRaceGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Components/AudioComponent.h"
#include "PlayerController/NetRacePlayerController.h"
#include "GameState/NetRaceGameState.h"
#include "PlayerState/NetRacePlayerState.h"

void ANetRaceGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    ANetRacePlayerController* NewPC = Cast<ANetRacePlayerController>(NewPlayer);
    if (true == ::IsValid(NewPC))
    {
        AlivePCs.Add(NewPC);
        UE_LOG(LogTemp, Log, TEXT("ANetRaceGameMode %d"), AlivePCs.Num());
    }
}

void ANetRaceGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    ANetRacePlayerController* ExitingPC = Cast<ANetRacePlayerController>(Exiting);
    if (true == ::IsValid(ExitingPC) && INDEX_NONE != AlivePCs.Find(ExitingPC))
    {
        AlivePCs.Remove(ExitingPC);
        DeadPCs.Add(ExitingPC);
    }
}


ANetRaceGameMode::ANetRaceGameMode()
{
    ReplicatedTime = 0.0f;

    // 시간 변수가 복제되도록 설정
    bReplicates = true;
}

/*
void ANetRaceGameMode::BeginPlay()
{
    FTimerHandle createHandler;
    GetWorld()->GetTimerManager().SetTimer(createHandler, FTimerDelegate::CreateLambda([&]() {
        StartDoolAudio();
        }), 20.0f, false);
    //StartDoolAudio();
}*/

//void ANetRaceGameMode::StartPlay()
void ANetRaceGameMode::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    // 시간 변수를 복제하도록 설정
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetRaceGameMode, ReplicatedTime);
}