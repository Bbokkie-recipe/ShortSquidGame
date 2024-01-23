// Fill out your copyright notice in the Description page of Project Settings.


#include "Doll/Doll.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/AudioComponent.h"
#include "PlayerController/NetRacePlayerController.h"
#include "GameState/NetRaceGameState.h"
#include "PlayerState/NetRacePlayerState.h"

ADoll::ADoll()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	HeadStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadStaticMesh"));
	HeadStaticMesh->SetupAttachment(RootComponent);
	BodyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyStaticMesh"));
	BodyStaticMesh->SetupAttachment(RootComponent);
	BowTieStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BowTieStaticMesh"));
	BowTieStaticMesh->SetupAttachment(RootComponent);
	DoolAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("DoolAudio"));
	DoolAudio->SetupAttachment(RootComponent);
	SearchAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("SearchAudio"));
	SearchAudio->SetupAttachment(RootComponent);
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void ADoll::BeginPlay()
{
	Super::BeginPlay();
	
	spareTime = 0.5f;
	SongALength = DoolAudio->Sound->GetDuration();
	SongBLength = SearchAudio->Sound->GetDuration();
	switchCooltime = SongBLength + spareTime;
	detectCooltime = SongALength + spareTime;
}

// Called every frame
void ADoll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ANetRaceGameState* myGameState = GetWorld()->GetGameState<ANetRaceGameState>();

	if (myGameState->SquidGameState == EGamePlayState::InProgress && myGameState != nullptr)
	{
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("InProgress in doll is working"));*/
		startTimer = true;
	}

	if (startTimer)
	{
		if (!isDetecting)
		{
			SwitchTimer(DeltaTime);
		}
		else if (isDetecting)
		{
			DetectingMode(DeltaTime);
		}
	}
}

void ADoll::SwitchTimer(float deltaTime)
{
	if (switchTimerTime < switchCooltime)
	{
		switchTimerTime += deltaTime;
		
	}
	else
	{
		switchTimerTime = 0.f;
		isDetecting = true;
		ANetRaceGameState* myGameState = GetWorld()->GetGameState<ANetRaceGameState>();
		if (myGameState)
		{
			myGameState->SearchMoving();
			UE_LOG(LogTemp, Warning, TEXT("Detect time"));
			//if (!HasAuthority()) {
			//	SearchAudio->Play();
			//}
			FVector NewRelativeLocation = HeadStaticMesh->GetRelativeLocation();
			NewRelativeLocation.Y += 20.f;
			HeadStaticMesh->SetRelativeLocation(NewRelativeLocation);
			HeadStaticMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		}
	}
}

void ADoll::DetectingMode(float deltaTime)
{

	if (detectTimerTime < detectCooltime)
	{
		detectTimerTime += deltaTime;

		//draw debug line 
	}
	else
	{
		detectTimerTime = 0.f;
		isDetecting = false;
		ANetRaceGameState* myGameState = GetWorld()->GetGameState<ANetRaceGameState>();
		if (myGameState)
		{
			myGameState->SingSong();
			//if (!HasAuthority()) {
			//	DoolAudio->Play();
			//}
			UE_LOG(LogTemp, Warning, TEXT("Sing Song"));
			FVector NewRelativeLocation = HeadStaticMesh->GetRelativeLocation();
			NewRelativeLocation.Y = 0.f;
			HeadStaticMesh->SetRelativeLocation(NewRelativeLocation);
			HeadStaticMesh->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		}
	}
}

void ADoll::StartDoolAudio()
{
	UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *GetActorNameOrLabel());
	//UE_LOG(LogTemp, Warning, TEXT("%s"), DoolAudio != nullptr ? *FString("Get Audio") : *FString("No Audio"));
	UWorld* World = GetWorld();
	if (World)
	{
		ANetRaceGameState* GameState = World->GetGameState<ANetRaceGameState>();
		if (GameState)
		{
			GameState->DoolSongState();
		}
	}
	DoolAudio->Play();

	GetWorldTimerManager().SetTimer(
		SongTimerHandle,
		this,
		&ADoll::StartSearchAudio,
		SongALength,
		false
	);
}

void ADoll::StartSearchAudio()
{
	UE_LOG(LogTemp, Warning, TEXT("StartSearchAudio"));

	UWorld* World = GetWorld();
	if (World)
	{
		// 서버에서 실행할 코드 추가
		ANetRaceGameState* GameState = World->GetGameState<ANetRaceGameState>();
		if (GameState)
		{
			GameState->DoolSearchState();
		}
	}
	SearchAudio->Play();

	GetWorldTimerManager().SetTimer(
		SongTimerHandle,
		this,
		&ADoll::StartDoolAudio,
		SongBLength,
		false
	);

}

void ADoll::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoll, SongALength);
	DOREPLIFETIME(ADoll, SongBLength);
}

