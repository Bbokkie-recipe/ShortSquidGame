// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NetworkGameInstance.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FSearchSessionDele, FString, roomName, FString, hostName, int32, currentPlayers, int32, maxPlayers, int32, ping, int32, sessionIdx); //필요한 데이터 6개

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FServerResponseDele);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindButtonStatus, bool, bIsEnabled);

UCLASS()
class SQUIDGAME_TEST_API UNetworkGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	IOnlineSessionPtr sessionInterface;
	//https://docs.unrealengine.com/4.27/ko/ProgrammingAndScripting/GameplayArchitecture/Properties/
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Mys")
	FSearchSessionDele onCreateSlot;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Mys")
	FServerResponseDele onNewSearchComplete;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "Mys")
	FFindButtonStatus onFindButtonToggle;

	void CreateSession(FString roomName, FString hostName, int32 playerCount);

	void FindSession();
	void JoinSession(int32 roomNumber);

	void SetSessionName(FString name);

	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	FORCEINLINE FString GetSessionName() const
	{
		return mySessionName.ToString();
	}
	void ExitSession();
private:
	FName mySessionName = FName("Squid Session");

	void OnCreatedSession(FName sessionName, bool bWasSuccessful);

	void OnFoundSessions(bool bWasSuccessful);

	void OnJoinedCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type result);

	void OnDestroyedSession(FName sessionName, bool bWasSuccessful);
};
