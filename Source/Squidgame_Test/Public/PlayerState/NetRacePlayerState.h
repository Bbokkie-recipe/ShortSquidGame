// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetRacePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API ANetRacePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ANetRacePlayerState();
	UFUNCTION(BlueprintCallable, Category = "SquidGame")
	void SetArrivalTime(float _ArrivalTime);

	UFUNCTION(BlueprintPure, Category = "SquidGame")
	float GetArrivalTime() const;

	UFUNCTION(Server, Reliable, WithValidation)
	void SetArrivalTime_Server(float _ArrivalTime);

	UFUNCTION(BlueprintCallable, Category = "SquidGame")
	void SetDeathTime(float _DeathTime);

	UFUNCTION(BlueprintPure, Category = "SquidGame")
	float GetDeathTime() const;

	UFUNCTION(Server, Reliable, WithValidation)
	void SetDeathTime_Server(float _DeathTime);

	UFUNCTION(BlueprintCallable, Category = "SquidGame")
	void SetRank(int32 _Rank);

	UFUNCTION(BlueprintPure, Category = "SquidGame")
	int32 GetRank() const;

	UFUNCTION(Server, Reliable, WithValidation)
	void SetRank_Server(float _Rank);

	UPROPERTY(Replicated)
	bool isDead = false;

	UPROPERTY(Replicated)
	bool HasCrossedFinishLine = false;

	UPROPERTY(Replicated)
	float ArrivalTime;

	UPROPERTY(Replicated)
	float DeathTime;

	UPROPERTY(Replicated)
	int32 Rank;
public:
	bool GetisDead();
	bool GetHasCrossedFinish();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
