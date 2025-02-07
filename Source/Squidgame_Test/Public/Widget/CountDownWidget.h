// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API UCountDownWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountdownText;

	UFUNCTION(BlueprintCallable)
	void UpdateCountdownText(int32 CountdownValue);
};
