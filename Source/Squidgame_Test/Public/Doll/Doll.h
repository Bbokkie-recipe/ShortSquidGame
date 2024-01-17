// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Doll.generated.h"

UCLASS()
class SQUIDGAME_TEST_API ADoll : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoll();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	
	void SwitchTimer(float deltaTime);

	void DetectingMode(float deltaTime);

	void StartDoolAudio();

	void StartSearchAudio();

	bool isDetecting = false;

private:
	UPROPERTY(VisibleAnywhere, Category = "Doll")
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Doll")
	TObjectPtr<class UStaticMeshComponent> HeadStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Doll")
	TObjectPtr<class UStaticMeshComponent> BodyStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Doll")
	TObjectPtr<class UStaticMeshComponent> BowTieStaticMesh;

	UPROPERTY(EditAnywhere, Category = "SquidAudio")
	UAudioComponent* DoolAudio; // ����ȭ���� �Ǿ����ϴ� ����

	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* SearchAudio; // ���� ����

private:
	float switchCooltime = 4.f;

	float detectCooltime = 3.f;

	float switchTimerTime = 0.f;

	float detectTimerTime = 0.f;

	float multiplier; 

	float angleAxis;

protected:
	FTimerHandle SongTimerHandle;

};
