// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NetRaceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API ANetRaceGameMode : public AGameMode // GameMode는 게임의 룰과 로직을 정의하고 서버에서 실행
{
	GENERATED_BODY()
public:
	ANetRaceGameMode();
	UPROPERTY(Replicated)
	float ReplicatedTime;

	UPROPERTY(EditAnywhere, Category = "SquidAudio")
	UAudioComponent* DoolAudio; // 무궁화꽃이 피었습니다 사운드

	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* SearchAudio; // 감시 사운드
protected:
	FTimerHandle SongTimerHandle;
	virtual void BeginPlay() override;
	void StartDoolAudio();
	void StartSearchAudio();
};
