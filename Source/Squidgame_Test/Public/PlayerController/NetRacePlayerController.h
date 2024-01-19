// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetRacePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API ANetRacePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ANetRacePlayerController();
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, Category = "Squid")
	TSubclassOf<class UCountDownWidget> CountDownWidget;
	UFUNCTION(Server, Reliable)
	void ServerStartCountdown();
	void StartCountdown();
	void UpdateCountdown();
	void CreateCountdownUI();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastCountdown();
public:
	class UCountDownWidget* CountDownUI;
	FTimerHandle CountdownTimerHandle;
	int32 CountdownValue;
	void ShowStateLog_forServer();
	void ShowStateLog();
};
