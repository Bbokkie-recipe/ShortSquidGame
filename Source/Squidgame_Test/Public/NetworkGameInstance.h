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

	void CreateSession(FString roomName, FString hostName, int32 playerCount);

private:
	
	void OncreatedSession(FName roomName, bool bWasSuccessful);


	
};
