// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

void UNetworkGameInstance::Init()
{
	Super::Init();

	sessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();
  
	if(sessionInterface != nullptr)
	{
		//서버로부터 이벤트 값을 받을 함수를 연결한다.
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnCreatedSession);
		//sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,
		   //&UNetworkGameInstance::OnFoundSessions);
	}
	
	sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnJoinedCompleted);
}


void UNetworkGameInstance::FindSession()
{
}

void UNetworkGameInstance::JoinSession(int32 roomNumber)
{
}

void UNetworkGameInstance::OnFoundSessions(bool bWasSuccessful)
{
}

void UNetworkGameInstance::OnJoinedCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
}

// <외부 ui에서 call할 함수> 서버에 세션 생성을 요청하는 함수
void UNetworkGameInstance::CreateSession(FString roomName, FString hostName, int32 playerCount)
{
	// 서버 생성 시의 옵션을 설정하기 위한 구조체 변수
	FOnlineSessionSettings sessionSettings;
	sessionSettings.bIsDedicated = false; // 리슨 서버
	sessionSettings.bAllowInvites = true; // 다른 사람 초대 수락여부
	sessionSettings.bAllowJoinInProgress = true;// 플레이 도중에 다른 사람 진입 가능
	sessionSettings.bAllowJoinViaPresence = true; // 현재 상태 기능 이용한 초대 기능
	sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false; // true는 랜연결, false는 스팀 등 다른 서버 경유
	sessionSettings.bShouldAdvertise = true; // 광고를 할꺼니: 다른 사람이 세션 찾으려고 하면 내가 세션 목록에 '검색'이 됨
	// 내가 초대하려고 하는 애들끼리면 private session이라 false가 되어야함
	sessionSettings.bUseLobbiesIfAvailable = true; 
	sessionSettings.NumPublicConnections = playerCount; 

	sessionSettings.Set(FName("Room Name"), roomName, EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);
	sessionSettings.Set(FName("Host Name"), hostName, EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);

	sessionInterface->CreateSession(0, mySessionName, sessionSettings); 
	UE_LOG(LogTemp, Warning, TEXT("Try to Create Session..."));
	UE_LOG(LogTemp, Warning, TEXT("Current Platform: %s"), *IOnlineSubsystem::Get()->GetSubsystemName().ToString());
}

void UNetworkGameInstance::OnCreatedSession(FName sessionName, bool bWasSuccessful) // 만들어졌으면bWasSuccessful true
{
	UE_LOG(LogTemp, Warning, TEXT("Session Name: %s"), *sessionName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Session Create: %s"), bWasSuccessful ? *FString("Success!") : *FString("Failed..."));
	// 멀티 플레이를 할 맵으로 이동한다.
	// 호스트가 자기가 레벨 파일을 가지고 있다(따라서 이동하고자 하는 레벨 파일의 경로를 써준다.
	GetWorld()->ServerTravel("/Game/Map/PlayMap?Listen", true);
}
