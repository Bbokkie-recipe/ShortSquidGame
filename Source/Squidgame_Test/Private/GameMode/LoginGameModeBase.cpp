// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LoginGameModeBase.h"
#include "Blueprint/UserWidget.h"

void ALoginGameModeBase::BeginPlay()
{
	if (loginWidget != nullptr) {
		UUserWidget* loginUI = CreateWidget<UUserWidget>(GetWorld(), loginWidget);
		if (loginUI != nullptr) {
			loginUI->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
	}
}