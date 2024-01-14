// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NetRaceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API ANetRaceGameMode : public AGameMode // GameMode�� ������ ��� ������ �����ϰ� �������� ����
{
	GENERATED_BODY()
public:
	ANetRaceGameMode();
	UPROPERTY(Replicated)
	float ReplicatedTime;

	UPROPERTY(EditAnywhere, Category = "SquidAudio")
	UAudioComponent* DoolAudio; // ����ȭ���� �Ǿ����ϴ� ����

	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* SearchAudio; // ���� ����
protected:
	FTimerHandle SongTimerHandle;
	virtual void BeginPlay() override;
	void StartDoolAudio();
	void StartSearchAudio();
};
