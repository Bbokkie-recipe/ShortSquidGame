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
	class UTextBlock* text_PlayerList;

	void ShowButtons();
	void UnShowButton();
	void AddPlayerList(FString playerName, float score, FString curState);
private:
	class ASquidgame_TestCharacter* player;
	FString playerList;
	UFUNCTION()
	void OnExitSession();
};
