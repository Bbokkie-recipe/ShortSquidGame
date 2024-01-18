// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/NetRacePlayerController.h"
#include "Widget/CountDownWidget.h"
ANetRacePlayerController::ANetRacePlayerController()
{
}

void ANetRacePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ANetRacePlayerController::StartCountdown()
{
    if (HasAuthority()) {
        ServerStartCountdown();
    }
}

void ANetRacePlayerController::UpdateCountdown()
{
    CountdownValue--;
    if (CountdownValue <= 0)
    {
        GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
        if (CountDownUI != nullptr)
        {
            CountDownUI->UpdateCountdownText(0);
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

void ANetRacePlayerController::ServerStartCountdown_Implementation()
{
    MulticastCountdown();
}

void ANetRacePlayerController::MulticastCountdown_Implementation()
{
    if (CountDownWidget != nullptr && CountDownUI == nullptr && this && this->IsLocalPlayerController()) {
        CountDownUI = CreateWidget<UCountDownWidget>(GetWorld(), CountDownWidget);
        if (CountDownUI != nullptr) {
            CountDownUI->AddToViewport();
        }
    }
    //UE_LOG(LogTemp, Warning, TEXT("ServerStartCountdown_Implementation called on PlayerController: %s"), *GetName());
    CountdownValue = 180;
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ANetRacePlayerController::UpdateCountdown, 1.0f, true);
    //UE_LOG(LogTemp, Warning, TEXT("Multicast MulticastCountdown_Implementation!")); // Å¬¶ó¸¸
}
