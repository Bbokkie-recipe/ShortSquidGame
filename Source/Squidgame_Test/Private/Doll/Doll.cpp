// Fill out your copyright notice in the Description page of Project Settings.


#include "Doll/Doll.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

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

