// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NetRaceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API ANetRaceGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ANetRaceGameMode();
	UPROPERTY(Replicated)
	float ReplicatedTime;
};
