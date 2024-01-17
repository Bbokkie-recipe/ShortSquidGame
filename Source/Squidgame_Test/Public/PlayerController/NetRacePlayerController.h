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
	void StartCountdown();
	void UpdateCountdown();
private:
	class UCountDownWidget* CountDownUI;
	FTimerHandle CountdownTimerHandle;
	int32 CountdownValue;
};
