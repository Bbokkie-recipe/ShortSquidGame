// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InGameWidget.h"
#include "../Squidgame_TestCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "NetworkGameInstance.h"
#include "GameState/NetRaceGameState.h"
#include "GameFramework/PlayerState.h"
#include "GameState/NetRaceGameState.h"

void UInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	player = GetOwningPlayerPawn<ASquidgame_TestCharacter>();
	btn_exitSession->OnClicked.AddDynamic(this, &UInGameWidget::OnExitSession);
	text_PlayerList->SetText(FText::FromString(TEXT("")));
}

void UInGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (player != nullptr) {
		ANetRaceGameState* NetGameState = GetWorld()->GetGameState<ANetRaceGameState>();
		if (NetGameState)
		{
			TArray<APlayerState*> players = NetGameState->GetMyPlayerList();
			playerList = "";
			for (APlayerState* p : players) {
				AddPlayerList(p->GetPlayerName(), p->GetScore());
			}
		}
		//TArray<APlayerState*> players = GetWorld()->GetGameState<ANetGameStateBase>()->GetMyPlayerList();
	}
}

void UInGameWidget::ShowButtons()
{
	btn_exitSession->SetVisibility(ESlateVisibility::Visible);
}

void UInGameWidget::UnShowButton()
{
	btn_exitSession->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameWidget::AddPlayerList(FString playerName, float score)
{
	playerList.Append(FString::Printf(TEXT("%s %d\n"), *playerName, (int32)score));
	text_PlayerList->SetText(FText::FromString(playerList));
}

void UInGameWidget::OnExitSession()
{
	GetGameInstance<UNetworkGameInstance>()->ExitSession();
}
