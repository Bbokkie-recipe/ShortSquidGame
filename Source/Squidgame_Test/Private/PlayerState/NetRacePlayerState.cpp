// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/NetRacePlayerState.h"
#include "Net/UnrealNetwork.h"

ANetRacePlayerState::ANetRacePlayerState()
{
	bIsReady = false;
	bIsHostStart = false;
	bReplicates = true;
}

// ArrivalTime
void ANetRacePlayerState::SetArrivalTime(float _ArrivalTime)
{
	SetArrivalTime_Server(_ArrivalTime);
}

void ANetRacePlayerState::SetArrivalTime_Server_Implementation(float _ArrivalTime)
{
	ArrivalTime = _ArrivalTime;
}

bool ANetRacePlayerState::SetArrivalTime_Server_Validate(float _ArrivalTime)
{
	return true;
}

float ANetRacePlayerState::GetArrivalTime() const
{
	return ArrivalTime;
}

void ANetRacePlayerState::SetDeathTime(float _DeathTime)
{
	SetDeathTime_Server(_DeathTime);
}

void ANetRacePlayerState::SetDeathTime_Server_Implementation(float _DeathTime)
{
	DeathTime = _DeathTime;
}

bool ANetRacePlayerState::SetDeathTime_Server_Validate(float _DeathTime)
{
	return true;
}

float ANetRacePlayerState::GetDeathTime() const
{
	return DeathTime;
}

void ANetRacePlayerState::SetRank(int32 _Rank)
{
	SetRank_Server(_Rank);
}

void ANetRacePlayerState::SetRank_Server_Implementation(float _Rank)
{
	Rank = _Rank;
}

bool ANetRacePlayerState::SetRank_Server_Validate(float _Rank)
{
	return true;
}

int32 ANetRacePlayerState::GetRank() const
{
	return Rank;
}


bool ANetRacePlayerState::GetisDead()
{
	return isDead;
}

bool ANetRacePlayerState::GetHasCrossedFinish()
{
	return HasCrossedFinishLine;
}

void ANetRacePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetRacePlayerState, isDead);
	DOREPLIFETIME(ANetRacePlayerState, HasCrossedFinishLine);
	DOREPLIFETIME(ANetRacePlayerState, ArrivalTime);
	DOREPLIFETIME(ANetRacePlayerState, DeathTime);
	DOREPLIFETIME(ANetRacePlayerState, Rank);
}