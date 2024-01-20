// Fill out your copyright notice in the Description page of Project Settings.


#include "StageActor/BlockLine.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "../Squidgame_TestCharacter.h"
#include "GameState/NetRaceGameState.h"
#include "PlayerState/NetRacePlayerState.h"

// Sets default values
ABlockLine::ABlockLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//blockComp->CreateDefaultSubobject<UBoxComponent>(TEXT("Block Line"));
	//blockComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//blockComp->SetCollisionObjectType(ECC_GameTraceChannel3);
	//blockComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	//lockComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	//SetRootComponent(blockComp);

	//meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh comp"));
	//meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//meshComp->SetupAttachment(RootComponent);

	//bReplicates = true;
	//SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ABlockLine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlockLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

