// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NetRaceGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Components/AudioComponent.h"

ANetRaceGameMode::ANetRaceGameMode()
{
    ReplicatedTime = 0.0f;

    // �ð� ������ �����ǵ��� ����
    bReplicates = true;
    //bReplicateInstigator = true;
    DoolAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("DoolAudio"));
    SearchAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("SearchAudio"));
}

void ANetRaceGameMode::BeginPlay()
{
    StartDoolAudio();
}

void ANetRaceGameMode::StartDoolAudio()
{
    if (DoolAudio)
    {
        DoolAudio->Play();
        float SongALength = DoolAudio->Sound->GetDuration();

        GetWorldTimerManager().SetTimer(
            SongTimerHandle,
            this,
            &ANetRaceGameMode::StartSearchAudio,
            SongALength,
            false
        );
    }
}

void ANetRaceGameMode::StartSearchAudio()
{
    UE_LOG(LogTemp, Warning, TEXT("StartSearchAudio"));
    if (SearchAudio)
    {
        SearchAudio->Play();
        float SongBLength = SearchAudio->Sound->GetDuration();

        GetWorldTimerManager().SetTimer(
            SongTimerHandle,
            this,
            &ANetRaceGameMode::StartDoolAudio,
            SongBLength,
            false
        );
    }
}

void ANetRaceGameMode::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    // �ð� ������ �����ϵ��� ����
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetRaceGameMode, ReplicatedTime);
}