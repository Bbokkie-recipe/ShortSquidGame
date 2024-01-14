// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/NetRacePlayerState.h"

bool ANetRacePlayerState::GetisDead()
{
	return isDead;
}

bool ANetRacePlayerState::GetHasCrossedFinish()
{
	return HasCrossedFinishLine;
}
