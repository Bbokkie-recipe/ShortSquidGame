// Fill out your copyright notice in the Description page of Project Settings.


#include "Doll/Laser.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Doll/Doll.h"
#include "GameState/NetRaceGameState.h"
#include "PlayerState/NetRacePlayerState.h"

// Sets default values
ALaser::ALaser()
{
	PrimaryActorTick.bCanEverTick = true;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
    BoxMesh->SetupAttachment(RootComponent);

    LaserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserEmitter"));
    LaserParticle->AttachToComponent(BoxMesh, FAttachmentTransformRules::KeepRelativeTransform);

	bHasFired = false;
	StartPoint = FVector::ZeroVector;
	EndPoint = FVector::ZeroVector;
}

void ALaser::BeginPlay()
{
	Super::BeginPlay();
    
}

void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bLaser)
    {
        Laser();
    }

}

void ALaser::Laser()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ANetRaceGameState* myGameState = GetWorld()->GetGameState<ANetRaceGameState>();
    ANetRacePlayerState* myPlayerState = PlayerController->GetPlayerState<ANetRacePlayerState>();
    
    if (PlayerController)
    {
        StartPoint = GetActorLocation();
        EndPoint = PlayerController->GetPawn()->GetActorLocation()+FVector(0,0,30);
        LaserParticle->SetBeamSourcePoint(0, StartPoint, 0);
        LaserParticle->SetBeamEndPoint(0, EndPoint);
    }

    if (myGameState != nullptr && myPlayerState != nullptr)
    {
        if (!(myPlayerState->GetPassed()) && !(myPlayerState->GetisDead()))
        {
            if (myGameState->SongState == ESongState::DollSong)
            {
                LaserParticle->Activate(true);
            }
            else if (myGameState->SongState == ESongState::SearchTime)
            {
                LaserParticle->Activate(false);
            }
            else
            {
                LaserParticle->Activate(true);
            }
        }
        else
        {
            LaserParticle->Activate(true);
            bLaser = false;
        }
    }
}

