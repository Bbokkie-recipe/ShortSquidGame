// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"
//https://youtu.be/eBN91xFteD4?feature=shared
UCLASS()
class SQUIDGAME_TEST_API ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Doll")
	TObjectPtr<class UStaticMeshComponent> BoxMesh;
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* LaserParticle;
	FVector StartPoint;
	FVector EndPoint;
	bool bHasFired;
	FVector PlayerLocation;
	bool bLaser = true;

public:
	void Laser();
};
