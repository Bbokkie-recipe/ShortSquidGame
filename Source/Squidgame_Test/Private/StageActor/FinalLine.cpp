// Fill out your copyright notice in the Description page of Project Settings.


#include "StageActor/FinalLine.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "../Squidgame_TestCharacter.h"
#include "GameState/NetRaceGameState.h"
#include "PlayerState/NetRacePlayerState.h"
#include "StageActor/BlockLine.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "PlayerController/NetRacePlayerController.h"

// Sets default values
AFinalLine::AFinalLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 오버랩으로 감지
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box col"));
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	boxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SetRootComponent(boxComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh comp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;
	//SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AFinalLine::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AFinalLine::OnOverlap);
}

// Called every frame
void AFinalLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStartTimer)
	{
		ClosedTimer(DeltaTime);
	}
}

void AFinalLine::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<ASquidgame_TestCharacter>(OtherActor);
	ANetRaceGameState* GameState = Cast<ANetRaceGameState>(GetWorld()->GetGameState());
	if (player != nullptr && GameState)
	{
		if(HasAuthority()) 
		{
			APlayerController* _PC = Cast<APlayerController>(player);
			ANetRacePlayerController* PlayerController = Cast<ANetRacePlayerController>(_PC);
			ANetRacePlayerState* PlayerState = player->GetPlayerState<ANetRacePlayerState>();
			if (PlayerController)
			{
				float time = PlayerController->CountdownValue;
				PlayerState->SetArrivalTime(time);
			}
			PlayerState->SetPassed();
			bStartTimer = true;
		}
	}
}

void AFinalLine::ClosedTimer(float deltaTime)
{
	if (closedTimer < closedCooltime)
	{
		closedTimer += deltaTime;
	}
	else 
	{
		closedTimer = 0;
		player->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
		bStartTimer = false;
	}
}

