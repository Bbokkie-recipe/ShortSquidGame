// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NetRaceGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Components/AudioComponent.h"
#include "PlayerController/NetRacePlayerController.h"

void ANetRaceGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    ANetRacePlayerController* NewPC = Cast<ANetRacePlayerController>(NewPlayer);
    if (true == ::IsValid(NewPC))
    {
        AlivePCs.Add(NewPC);
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
    //bReplicateInstigator = true;
    DoolAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("DoolAudio"));
    SearchAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("SearchAudio"));
}

void ANetRaceGameMode::BeginPlay()
{
    FTimerHandle createHandler;
    GetWorld()->GetTimerManager().SetTimer(createHandler, FTimerDelegate::CreateLambda([&]() {
        StartDoolAudio();
        }), 20.0f, false);
    //StartDoolAudio();
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
    // 시간 변수를 복제하도록 설정
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetRaceGameMode, ReplicatedTime);
}