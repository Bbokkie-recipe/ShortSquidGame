// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoginGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SQUIDGAME_TEST_API ALoginGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, Category = "my")
	TSubclassOf<UUserWidget> loginWidget;
};
