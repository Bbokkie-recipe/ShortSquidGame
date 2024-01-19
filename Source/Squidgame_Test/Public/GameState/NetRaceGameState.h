// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NetRaceGameState.generated.h"

/**
 * 
 */

UENUM()
enum class ESongState : uint8
{
	None,
	DollSong,
	SearchTime,
};

UENUM()
enum class EGamePlayState : uint8
{
	WaitingToStart,
	InProgress,
	GameOver,
};
UCLASS()
class SQUIDGAME_TEST_API ANetRaceGameState : public AGameState //GameState는 게임 상태를 저장하고 모든 플레이어에게 동기화
{
	GENERATED_BODY()
public:
	TArray<class APlayerState*> GetMyPlayerList();
	static bool AscendingByString(const APlayerState& ps1, const APlayerState& ps2);
	static bool DescendingByString(const APlayerState& ps1, const APlayerState& ps2);
	static bool AscendingByScore(const APlayerState& ps1, const APlayerState& ps2);
	static bool DescendingByScore(const APlayerState& ps1, const APlayerState& ps2);
public:
	ANetRaceGameState();

	void SearchDoll();
public:
	UPROPERTY(Replicated)
	int32 GameStartTime;
	UPROPERTY(Replicated)
	int32 GameEndTime;
	UPROPERTY(Replicated)
	EGamePlayState SquidGameState;
	UPROPERTY(Replicated)
	ESongState SongState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Squid_Game")
	float TimeLimitInSeconds;

	bool ReadyPlay();
	void SingSong();
	void SearchMoving();

protected:
	UPROPERTY()
	class ADoll* FoundDoll;
public:
	bool IsGameOverCondition() const;
	void StartGame();
	void EndGame();
	void DoolSongState();
	void DoolSearchState();
	int32 GetElapsedGameTime() const;
	FString GetGameStateAsString();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
