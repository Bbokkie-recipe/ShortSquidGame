// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinalLine.generated.h"

UCLASS()
class SQUIDGAME_TEST_API AFinalLine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFinalLine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* meshComp;
private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
