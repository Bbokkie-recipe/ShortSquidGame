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

    // �ð� ������ �����ǵ��� ����
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
    // �ð� ������ �����ϵ��� ����
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetRaceGameMode, ReplicatedTime);
}