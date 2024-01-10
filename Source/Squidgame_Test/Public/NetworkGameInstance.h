// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NetworkGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API UNetworkGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	IOnlineSessionPtr sessionInterface;

	void CreateSession(FString roomName, int32 playerCount);

private:
	FName mySessionName = FName("Sesac_Session");

	void OncreatedSession(FName sessionName,  bool bWasSuccessful);
	
};
