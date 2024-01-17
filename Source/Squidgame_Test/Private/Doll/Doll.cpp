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
	
	multiplier = 50.0f;
}

// Called when the game starts or when spawned
void ADoll::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isDetecting)
	{
		SwitchTimer(DeltaTime);
	}
	else if (isDetecting)
	{
		DetectingMode(DeltaTime);
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
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("DetectMode!"));
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
	}
	else
	{
		detectTimerTime = 0.f;
		isDetecting = false;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Waiting.."));
	}
}

void ADoll::StartDoolAudio()
{
	if (DoolAudio)
	{
		DoolAudio->Play();
		float SongALength = DoolAudio->Sound->GetDuration();

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
		SearchAudio->Play();
		float SongBLength = SearchAudio->Sound->GetDuration();

		GetWorldTimerManager().SetTimer(
			SongTimerHandle,
			this,
			&ADoll::StartDoolAudio,
			SongBLength,
			false
		);
	}
}
