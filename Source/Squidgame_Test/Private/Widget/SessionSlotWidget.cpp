// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SessionSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "NetworkGameInstance.h"

void USessionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	btn_join->OnClicked.AddDynamic(this, &USessionSlotWidget::OnClickedJoinButton);
	gi = GetGameInstance<UNetworkGameInstance>();
}

void USessionSlotWidget::SetSessionInfo(const FString roomName, const FString hostName, const int32 curPlayer, const int32 maxPlayer, const int32 pingSpeed, const int32 idx)
{
	text_roomName->SetText(FText::FromString(roomName));
	text_hostName->SetText(FText::FromString(hostName));
	text_playerCount->SetText(FText::FromString(FString::Printf(TEXT("(%d/%d)"), curPlayer, maxPlayer)));
	text_ping->SetText(FText::AsNumber(pingSpeed));
	// 몇번째 만들어진건지. 알아보기 위함
	roomIndex = idx;
}

void USessionSlotWidget::OnClickedJoinButton()
{
	if (gi != nullptr) {
		gi->JoinSession(roomIndex);
	}
}
