// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ReadyWidget.h"
#include "Components/TextBlock.h"
#include "../Squidgame_TestCharacter.h"
#include "GameState/NetRaceGameState.h"

void UReadyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	player = GetOwningPlayerPawn<ASquidgame_TestCharacter>();
	text_ready->SetText(FText::AsNumber(0));
}

void UReadyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (player != nullptr) {
		AController* OwningController = player->GetController();
		if (OwningController)
		{
			if (player->GetLocalRole() == ENetRole::ROLE_Authority) {
				text_ready->SetText(FText::FromString("Start"));
			}
			else {
				text_ready->SetText(FText::FromString("Ready"));
			}
			ANetRaceGameState* PlayerState = Cast<ANetRaceGameState>(OwningController->PlayerState);

			if (PlayerState)
			{
				FString PlayerName = PlayerState->GetName();
			}
		}
	}
}
