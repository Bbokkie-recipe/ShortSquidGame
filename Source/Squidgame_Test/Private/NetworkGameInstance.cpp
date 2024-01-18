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
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnCreatedSession);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnFoundSessions);
		sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnDestroyedSession);
	}
	
	sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnJoinedCompleted);
}

void UNetworkGameInstance::SetSessionName(FString name)
{
	mySessionName = FName(*name);
}

void UNetworkGameInstance::FindSession()
{
	onFindButtonToggle.Broadcast(false);
	sessionSearch = MakeShareable(new FOnlineSessionSearch()); // native c++ 스팀에서 제공
	sessionSearch->bIsLanQuery = true;// 랜 연결로 된 것만 찾을 것인가?
	sessionSearch->MaxSearchResults = 10;//최대 몇개까지 찾을 것인가?
	//SEARCH_PRESENCE이 트루인 경우에만 
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Type::Equals); // 특정 값과 일치하는 것만 // 문자열 조건부 검색
	// 이퀄로 검색돌려줌 원하는 검색 조건을 추가할 수 있다

	// 서버에 세션 검색을 요청
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
	//ToSharedRef 매개변수 참조자로 넣는다는건 변수 안에다 값을 채워주겠다는 뜻
	//참조형태 넣어서 요청하면 다음 함수안에 알아서 찾은 결과들을 채워서 반환해줌
}

void UNetworkGameInstance::JoinSession(int32 roomNumber)
{
	sessionInterface->JoinSession(0, mySessionName, sessionSearch->SearchResults[roomNumber]);
}

void UNetworkGameInstance::OnFoundSessions(bool bWasSuccessful)
{
	TArray<FOnlineSessionSearchResult> results = sessionSearch->SearchResults;
	UE_LOG(LogTemp, Warning, TEXT("Find Results: %s"), bWasSuccessful ? *FString("Success!") : *FString("Failed..."));
	if (bWasSuccessful) {
		int32 sessionNum = results.Num();
		UE_LOG(LogTemp, Warning, TEXT("Session Count : %d"), results.Num());
		// 응답 성공이면 클리어
		onNewSearchComplete.Broadcast();
		for (int32 i = 0; i < results.Num(); i++) {
			FString foundRoomName;
			// 전형적인 참조의 형태로 변수 반환이 아니라 인자로 넣으면 채워넣어줌
			results[i].Session.SessionSettings.Get(FName("Room Name"), foundRoomName);
			FString foundHostName;
			results[i].Session.SessionSettings.Get(FName("Host Name"), foundHostName);
			int32 maxPlayerCount = results[i].Session.SessionSettings.NumPublicConnections;
			int32 currentPlayerCount = maxPlayerCount - results[i].Session.NumOpenPublicConnections; // 랜 연결때 오류가 있을 수 있다 스팀연결때는 잘뜬다
			// ping은 회선 속도
			// 회선 속도가 가장 느린 사람에 맞춘다.
			int32 pingSpeed = results[i].PingInMs;
			UE_LOG(LogTemp, Warning, TEXT("RoomName : %s\n Host Name : %s\n(%d/%d)\nPing: %d ms\n\n"), *foundRoomName, *foundHostName, currentPlayerCount, maxPlayerCount, pingSpeed);

			//델리게이트 이벤트 실행하기 : 알릴 정보들 넘겨줌
			onCreateSlot.Broadcast(foundRoomName, foundHostName, currentPlayerCount, maxPlayerCount, pingSpeed, i);
		}
		onFindButtonToggle.Broadcast(true);
	}
}

void UNetworkGameInstance::OnJoinedCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	UE_LOG(LogTemp, Warning, TEXT("Joined Session: %s"), *sessionName.ToString());
	switch (result)
	{
	case EOnJoinSessionCompleteResult::Success:
	{
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		FString url;
		//NAME_GamePort라고 하면
		sessionInterface->GetResolvedConnectString(sessionName, url, NAME_GamePort);// 특정 세션에 이름을 넣어주면 그 ip 주소를 문자열로 받아옴
		//pc->ClientTravel();
		UE_LOG(LogTemp, Warning, TEXT("url: %s Joihn Success!"), *url);
		pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		break;
	}
	case EOnJoinSessionCompleteResult::SessionIsFull:
		UE_LOG(LogTemp, Warning, TEXT("Session Is Full"));
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		UE_LOG(LogTemp, Warning, TEXT("Session Does Not Exist!"));
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		UE_LOG(LogTemp, Warning, TEXT("Could Not Retrieve Address!"));
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		UE_LOG(LogTemp, Warning, TEXT("you are Already In Session!"));
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		UE_LOG(LogTemp, Warning, TEXT("Unknown Error!"));
		break;
	default:
		break;
	}
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
	sessionSettings.bShouldAdvertise = true; // 광고: 다른 사람이 세션 찾으려고 하면 내가 세션 목록에 '검색'이 됨
	// 내가 초대하려고 하는 애들끼리면 private session이라 false
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

void UNetworkGameInstance::ExitSession()
{
	sessionInterface->DestroySession(mySessionName);
}

void UNetworkGameInstance::OnDestroyedSession(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Destroy Session: %s"), bWasSuccessful == true ? *FString("Success!") : *FString("Faiiled..."));

	if (bWasSuccessful) {
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		if (pc != nullptr) {
			pc->ClientTravel(FString("/Game/Maps/Lobby"), ETravelType::TRAVEL_Absolute);
		}
	}
}