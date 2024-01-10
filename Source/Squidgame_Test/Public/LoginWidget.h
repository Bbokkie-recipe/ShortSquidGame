// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;


public:

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category="MySettings")
	class UEditableText* eText_roomName;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category="MySettings")
	class UEditableText* eText_hostName;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category="MySettings")
	class USlider* s1_maxPlayers;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
	class UTextBlock* text_maxPlayers;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
	class UButton* btn_create;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
	class UButton* btn_back;


};
