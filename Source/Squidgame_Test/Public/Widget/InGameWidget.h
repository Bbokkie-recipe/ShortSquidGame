// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySet")
	class UButton* btn_exitSession;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySet")
	class UButton* btn_exitGame;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySet")
	class UTextBlock* text_PlayerList;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySet")
	class UTextBlock* text_SPlayerList;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySet")
	class UTextBlock* text_DPlayerList;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UWidgetSwitcher* widgetSwicher;

	void ShowExitButtons();
	void UnShowButton();
	void AddPlayerList(FString playerName, float score, FString curState);
	void ShowGameResult();
	void AddPassPlayerList(FString playerName, FString curState);//int32 rank, 
	void AddNonPassPlayerList(FString playerName, FString curState);//int32 rank, 
private:
	class ASquidgame_TestCharacter* player;
	FString playerList;
	FString playerNonPassedList;
	UFUNCTION()
	void OnExitSession();
};
