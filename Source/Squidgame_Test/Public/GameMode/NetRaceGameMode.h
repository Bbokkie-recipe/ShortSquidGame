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
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SquidAGameMode", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<class ANetRacePlayerController>> AlivePCs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SquidAGameMode", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<class ANetRacePlayerController>> DeadPCs;
public:
	ANetRaceGameMode();
	UPROPERTY(Replicated)
	float ReplicatedTime;
};
