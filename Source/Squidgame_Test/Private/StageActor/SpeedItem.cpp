// Fill out your copyright notice in the Description page of Project Settings.


#include "StageActor/SpeedItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Squidgame_Test/Squidgame_TestCharacter.h"

// Sets default values
ASpeedItem::ASpeedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = boxComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	meshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASpeedItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpeedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpeedItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASquidgame_TestCharacter* player = Cast<ASquidgame_TestCharacter>(OtherActor);
	if (player != nullptr)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("speed Item!"));

		player->bRunnable = true;
		this->Destroy();
	}
}

