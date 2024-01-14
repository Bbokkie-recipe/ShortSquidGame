// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NetRaceGameMode.h"
#include "Net/UnrealNetwork.h"

ANetRaceGameMode::ANetRaceGameMode()
{
    ReplicatedTime = 0.0f;

    // 시간 변수가 복제되도록 설정
    bReplicates = true;
    //bReplicateInstigator = true;
}

void ANetRaceGameMode::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    // 시간 변수를 복제하도록 설정
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetRaceGameMode, ReplicatedTime);
}