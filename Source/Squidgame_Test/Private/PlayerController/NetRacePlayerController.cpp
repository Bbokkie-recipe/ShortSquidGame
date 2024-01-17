// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/NetRacePlayerController.h"
#include "Widget/CountDownWidget.h"
ANetRacePlayerController::ANetRacePlayerController()
{
}

void ANetRacePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (CountDownWidget != nullptr && this && this->IsLocalPlayerController()) {
		CountDownUI = CreateWidget<UCountDownWidget>(GetWorld(), CountDownWidget);
		if (CountDownUI != nullptr) {
			CountDownUI->AddToViewport();
		}
	}
}

void ANetRacePlayerController::StartCountdown()
{
	CountdownValue = 180;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ANetRacePlayerController::UpdateCountdown, 1.0f, true);
}

void ANetRacePlayerController::UpdateCountdown()
{
    CountdownValue--;
    if (CountdownValue <= 0)
    {
        GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
        CountDownUI->UpdateCountdownText(0);
        if (CountDownUI != nullptr)
        {
            CountDownUI->RemoveFromParent();
            CountDownUI = nullptr;
        }
    }
    else
    {
        if (CountDownUI != nullptr)
        {
            CountDownUI->UpdateCountdownText(CountdownValue);
        }
    }
}