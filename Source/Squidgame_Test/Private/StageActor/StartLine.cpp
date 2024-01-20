// Fill out your copyright notice in the Description page of Project Settings.


#include "StageActor/StartLine.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "../Squidgame_TestCharacter.h"
#include "GameState/NetRaceGameState.h"
#include "PlayerState/NetRacePlayerState.h"


// Sets default values
AStartLine::AStartLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Start Line Box"));
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);
	boxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SetRootComponent(boxComp);

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AStartLine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStartLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ANetRaceGameState* myGameState = GetWorld()->GetGameState<ANetRaceGameState>();
	if (myGameState->SquidGameState == EGamePlayState::InProgress && myGameState != nullptr)
	{
		boxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}


