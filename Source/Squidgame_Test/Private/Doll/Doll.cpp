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
	SearchAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("SearchAudio"));
	

}

// Called when the game starts or when spawned
void ADoll::BeginPlay()
{
	Super::BeginPlay();

	SongALength = DoolAudio->Sound->GetDuration();
	SongBLength = SearchAudio->Sound->GetDuration();

	switchCooltime = SongBLength;
	detectCooltime = SongALength;
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
	}
}

void ADoll::DetectingMode(float deltaTime)
{
	/*angleAxis = deltaTime * multiplier;

	FRotator NewRotation = FRotator(0, angleAxis, 0);

	HeadStaticMesh->SetRelativeRotation(NewRotation);
	doll 머리 움직이기는 것 구현하기 */


	if (detectTimerTime < detectCooltime)
	{
		detectTimerTime += deltaTime;

		//draw debug line 
	}
	else
	{
		detectTimerTime = 0.f;
		isDetecting = false;
	}
}

void ADoll::StartDoolAudio()
{
	if (DoolAudio)
	{
		if (GetLocalRole() == ENetRole::ROLE_Authority) {
			ANetRaceGameState* GameState = GetWorld()->GetGameState<ANetRaceGameState>();
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
}

void ADoll::StartSearchAudio()
{
	UE_LOG(LogTemp, Warning, TEXT("StartSearchAudio"));
	if (SearchAudio)
	{
		if (GetLocalRole() == ENetRole::ROLE_Authority) {
			ANetRaceGameState* GameState = GetWorld()->GetGameState<ANetRaceGameState>();
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
}
