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
	UPROPERTY(BlueprintReadOnly) // �������Ʈ�� �ڽ� ��ü�ϱ� private�δ� ���� ���ϴ� public���� �÷���
		class UNetworkGameInstance* gi;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UEditableText* eText_roomName; // ��ǲ�ڽ��� ������ �ؽ�Ʈ UI ���� ������Ʈ �������� ǥ���ϱ� ���� ���ξ ���̴� ��찡 ���� �ִ�
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UEditableText* eText_hostName;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
	class USlider* sl_maxPlayers;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UTextBlock* text_maxPlayers;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UButton* btn_create;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UButton* btn_back;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UWidgetSwitcher* ws_widgetSwicher;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UButton* btn_Move2CreatePanel;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UButton* btn_MoveToFindPanel;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UScrollBox* sb_roomList;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UButton* btn_back2;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UButton* btn_findSessions;
	UPROPERTY(EditAnywhere, Category = "My")
	TSubclassOf<class USessionSlotWidget> slotWidget;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UEditableText* editText_UserName;
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = "MySettings")
	class UButton* btn_next;
private:

	UFUNCTION()
	void OnClickedCreateButton();

	UFUNCTION()
	void OnClickedBackButton();

	UFUNCTION()
	void OnSliderMoved(float value);

	UFUNCTION()
	void OnClickedMoveToCreate();

	UFUNCTION()
	void OnClickedMoveToFind();

	UFUNCTION()
	void OnClickedFindSessionsButton();

	UFUNCTION()
	void OnSlotCreated(FString roomName, FString hostName, int32 currentPlayers, int32 maxPlayers, int32 ping, int32 sessionIdx);

	UFUNCTION()
	void OnClearScrollBox();

	UFUNCTION()
	void FindButtonOnOff(bool on);

	UFUNCTION()
	void SetUserNameAndNext();
};
