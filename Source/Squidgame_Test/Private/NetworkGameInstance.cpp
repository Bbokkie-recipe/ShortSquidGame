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
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,
		   &UNetworkGameInstance::OncreatedSession);
		//sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,
		   //&UNetworkGameInstance::OnFoundSessions);
	}
	
	FTimerHandle createHandler;
	
	// 내 세션을 서버에 생성 요청(2초 지연)-주석처리함
	//GetWorld()->GetTimerManager().SetTimer(createHandler, FTimerDelegate::CreateLambda([&]() 
	//	{
	//		CreateSession(mySessionName.ToString(), 5);
	//	}), 2.0f, false);
}

	//서버에 세션 생성을 요청하는 함수
void UNetworkGameInstance::CreateSession(FString roomName, FString hostName, int32 playerCount)
{
	//서버 생성시의 옵션을 설정하기 위한 구조체 변수
	FOnlineSessionSettings sessionSettings;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bAllowInvites = true; //초대가능 여부
	sessionSettings.bAllowJoinInProgress = true; //게임 진행중에도 입장가능한가Y
	sessionSettings.bAllowJoinViaPresence = true;
	sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	//접속하는 방법 -> 스팀, LAN 등이 있는데, 위의 명령어는 해당 조건에 따라 바뀐다
	//스팀이면 Steam 을 반환하고 LAN 연결이면 null값을 반환한다.
	sessionSettings.bShouldAdvertise = true;
	//다른사람이 session 을 찾으려고 할 때 session 목록에 해당 session 을 공개할 것인가?
	sessionSettings.bUseLobbiesIfAvailable = true;
	//로비를 사용할 것인가?
	sessionSettings.NumPublicConnections = playerCount;

	//몇 년이 지나도, 다른 사람이 봐도 알 수 있게끔 변수이름을 설정해야한다.

	// 커스텀 설정 값을 추가하기
	sessionSettings.Set(FName("Room Name"), roomName, EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);
	sessionSettings.Set(FName("Host Name"), hostName, EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);

	sessionInterface->CreateSession(0, mySessionName, sessionSettings);
	UE_LOG(LogTemp, Warning, TEXT("Try to create session..."));
	UE_LOG(LogTemp, Warning, TEXT("Current Platform: %s"), *IOnlineSubsystem::Get()->GetSubsystemName().ToString());

}

	//서버로부터 들어온 결과 (세션 생성 결과)이벤트 함수
void UNetworkGameInstance::OncreatedSession(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Session Name : %s"), *sessionName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Session Create : %s"),
	   bWasSuccessful ? *FString("Success!") : *FString("Failed..."));
}
