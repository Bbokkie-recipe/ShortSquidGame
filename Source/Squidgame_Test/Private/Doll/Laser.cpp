// Fill out your copyright notice in the Description page of Project Settings.


#include "Doll/Laser.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

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
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    LaserParticle->Activate(true);
    if (PlayerController)
    {
        PlayerLocation = PlayerController->GetPawn()->GetActorLocation();

        StartPoint = GetActorLocation();
        EndPoint = PlayerLocation;

        LaserParticle->SetBeamSourcePoint(0, StartPoint, 0);
        LaserParticle->SetBeamEndPoint(0, EndPoint);

        LaserParticle->Activate(true);
    }
}

void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (PlayerController)
    {
        PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
    }
    LaserParticle->SetBeamSourcePoint(0, StartPoint, 0);
    LaserParticle->SetBeamTargetPoint(0, PlayerLocation, 0);
}

