// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ReadyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Squidgame_TestCharacter.h"
#include "GameState/NetRaceGameState.h"
#include "PlayerState/NetRacePlayerState.h"
#include "GameMode/NetRaceGameMode.h"
void UReadyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	readyButton->OnClicked.AddDynamic(this, &UReadyWidget::OnClickedReadyButton);
	player = GetOwningPlayerPawn<ASquidgame_TestCharacter>();
	text_ready->SetText(FText::AsNumber(0));
	HideNotifyNotReady();
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

void UReadyWidget::OnClickedReadyButton()
{
	// �����̸�
	   // 1���̸�
	   // 1���� �ƴϸ�
	// �����ڸ�
	if (player != nullptr) {
		AController* OwningController = player->GetController();
		if (OwningController)
		{
			if (player->GetLocalRole() == ENetRole::ROLE_Authority) {
				ANetRacePlayerState* PlayerState = Cast<ANetRacePlayerState>(OwningController->PlayerState);
				PlayerState->SetReady();
				ANetRaceGameMode* GameMode = Cast<ANetRaceGameMode>(GetWorld()->GetAuthGameMode());
				if (GameMode)
				{
					bool isReadyComplete = GameMode->ReadyPlay();
					if (isReadyComplete)
					{
						// ���� ����
						RemoveFromParent();
					}
					else {
						ShowNotifyNotReady();
					}
				}
			}
			else {
				ANetRacePlayerState* PlayerState = Cast<ANetRacePlayerState>(OwningController->PlayerState);
				PlayerState->SetReady();
				readyButton->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UReadyWidget::HideNotifyNotReady()
{
	notifyNotready->SetVisibility(ESlateVisibility::Hidden);
}

void UReadyWidget::ShowNotifyNotReady()
{
	int32 zero = 0;
	ANetRaceGameMode* GameMode = Cast<ANetRaceGameMode>(GetWorld()->GetAuthGameMode());
	TArray<TObjectPtr<APlayerState>> Players = GameMode->GameState->PlayerArray;
	for (APlayerState* PlayerState : Players) {
		ANetRacePlayerState* PS = Cast<ANetRacePlayerState>(PlayerState);
		if (PS && !(PS->bIsReady))
		{
			zero++;
			notifyNotready->SetText(FText::FromString(PS->GetActorNameOrLabel()));
			text_ready->SetText(FText::AsNumber(zero));
        }
	}
	//notifyNotready->SetText(FText::AsNumber(zero));
	//notifyNotready->SetText(FText::AsNumber(zero));
	notifyNotready->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(NotifyTimerHandle, this, &UReadyWidget::HideNotifyNotReady, 3.0f, false);
}