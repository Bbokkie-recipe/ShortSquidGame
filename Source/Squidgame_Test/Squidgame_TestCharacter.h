// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Squidgame_TestCharacter.generated.h"
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASquidgame_TestCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

public:
	ASquidgame_TestCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Run)
	float playerSpeed = 500.f;

	UPROPERTY(BlueprintReadOnly)
	bool bRunning = false;

	float coolTime = 1.0f;

	float runTimer;

	bool runReady = true;

	bool bDetecting = true;

	bool bAlive = true;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Run();

	void RunCooltimeTimer(float deltaTime);

	void CheckMovement(bool isDetecting);

	void StartDetect();

	void Dead();

	bool IsProgress();

	bool IsPassed();
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	UPROPERTY(EditAnywhere, Category = "SquidGame")
	TSubclassOf<class UReadyWidget> readyWidget;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DeadAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class USoundBase* GunFiredSound;

private:
	class UReadyWidget* readyUI;
	enum ENetRole localRole;
	enum ENetRole remoteRole;

	FVector currentPos, originPos;
	FRotator currentRot, originRot;

public:
	UPROPERTY(EditAnywhere, Category = "Squid")
	TSubclassOf<class UInGameWidget> InGameWidget;

	void PrintInfoLog();
private:
	class UInGameWidget* InGameUI;

public:
	UFUNCTION(Server, Reliable)
	void ServerDieProcess();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDieProcess();
};

