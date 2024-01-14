// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NetRaceGameState.generated.h"

/**
 * 
 */
UENUM()
enum class EGamePlayState : uint8
{
	WaitingToStart,
	InProgress,
	GameOver,
};
UCLASS()
class SQUIDGAME_TEST_API ANetRaceGameState : public AGameState
{
	GENERATED_BODY()
public:
	ANetRaceGameState();
public:
	int32 GameStartTime;

	int32 GameEndTime;

	EGamePlayState GameState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Squid_Game")
	float TimeLimitInSeconds;
protected:
	virtual void DefaultTimer() override;

public:
	bool IsGameOverCondition() const;
	void StartGame();
	void EndGame();
	int32 GetElapsedGameTime() const;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
