// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NetRaceGameMode.h"
#include "Net/UnrealNetwork.h"

ANetRaceGameMode::ANetRaceGameMode()
{
    ReplicatedTime = 0.0f;

    // �ð� ������ �����ǵ��� ����
    bReplicates = true;
    //bReplicateInstigator = true;
}

void ANetRaceGameMode::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    // �ð� ������ �����ϵ��� ����
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetRaceGameMode, ReplicatedTime);
}