// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadyWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API UReadyWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "SquidGame")
	class UTextBlock* text_ready;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "SquidGame")
	class UTextBlock* notifyNotready;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "SquidGame")
	class UButton* readyButton;

	FTimerHandle NotifyTimerHandle;
private:
	class ASquidgame_TestCharacter* player;
	UFUNCTION()
	void OnClickedReadyButton();
	void HideNotifyNotReady();
	void ShowNotifyNotReady();
};
