// Copyright Epic Games, Inc. All Rights Reserved.

#include "Squidgame_TestCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "Widget/ReadyWidget.h"
#include "Doll/Doll.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "PlayerState/NetRacePlayerState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/InGameWidget.h"
#include "GameState/NetRaceGameState.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASquidgame_TestCharacter

ASquidgame_TestCharacter::ASquidgame_TestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_EngineTraceChannel1, ECR_Overlap);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	runTimer = 0;

	bReplicates = true;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ASquidgame_TestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	localRole = GetLocalRole();
	remoteRole = GetRemoteRole();
	if (readyWidget != nullptr && GetController() && GetController()->IsLocalPlayerController()) {
		readyUI = CreateWidget<UReadyWidget>(GetWorld(), readyWidget);
		if (readyUI != nullptr) {
			readyUI->AddToViewport();
			PlayerController->SetShowMouseCursor(true);
		}
	}
	if (InGameWidget != nullptr && GetController() && GetController()->IsLocalPlayerController()) {
		InGameUI = CreateWidget<UInGameWidget>(GetWorld(), InGameWidget);
		if (InGameUI != nullptr) {
			InGameUI->AddToViewport();
			InGameUI->UnShowButton();
		}
	}
}

bool ASquidgame_TestCharacter::IsProgress()
{
	ANetRaceGameState* myGameState = GetWorld()->GetGameState<ANetRaceGameState>();
	if (myGameState->SquidGameState == EGamePlayState::InProgress && myGameState != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ASquidgame_TestCharacter::IsPassed()
{
	if (GetController()->IsLocalController())
	{
		ANetRacePlayerState* myPlayerState = GetController()->GetPlayerState<ANetRacePlayerState>();

		if (myPlayerState->GetPassed())
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

void ASquidgame_TestCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (HasAuthority()) {
		if (IsProgress() && !IsPassed())
		{
			if (bAlive)
			{
				StartDetect();
			}
		}

		if (!runReady && bRunnable)
		{
			RunCooltimeTimer(DeltaSeconds);
		}
	}
	if (GetLocalRole() == ENetRole::ROLE_Authority) {
		ANetRaceGameState* GameState = GetWorld()->GetGameState<ANetRaceGameState>();
		if (GameState)
		{
			if (GameState->EndSquidPlay()) {
				float DelayTime = 2.0f;
				FTimerHandle TimerHandle;

				GetWorldTimerManager().SetTimer(TimerHandle, this, &ASquidgame_TestCharacter::ServerShowResult, DelayTime, false);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASquidgame_TestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASquidgame_TestCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASquidgame_TestCharacter::Look);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ASquidgame_TestCharacter::Run);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASquidgame_TestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASquidgame_TestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASquidgame_TestCharacter::Run()
{
	if (runReady)
	{
		if (bRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.f;
			bRunning = false;
			runReady = false;
		/*	if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Running!"));*/
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 250.f;
			bRunning = true;
			runReady = false;
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Walking!"));*/
		}
	}
}

void ASquidgame_TestCharacter::RunCooltimeTimer(float deltaTime)
{
	
	if (runTimer < runCoolTime)
	{
		runTimer += deltaTime;
	}
	else if (runTimer >= runCoolTime)
	{
		runReady = true;
		runTimer = 0;
	}
}


void ASquidgame_TestCharacter::StartDetect()
{
	for (TActorIterator<ADoll>_doll(GetWorld()); _doll; ++_doll)
	{
		if (_doll)
		{
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("dollActor is working"));*/
			
			bDetecting = _doll->isDetecting;
			CheckMovement(bDetecting);
		}
	}
}

void ASquidgame_TestCharacter::CheckMovement(bool isDetecting)
{
	currentPos = GetActorLocation();
	currentRot = GetActorRotation();


	if (isDetecting)
	{
		FVector subtractVector = UKismetMathLibrary::Subtract_VectorVector(currentPos, originPos);

		if (subtractVector != FVector(0, 0, 0))
		{
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Dead"));*/
			ANetRacePlayerState* MyPlayerState = Cast<ANetRacePlayerState>(GetController()->PlayerState);
			if (MyPlayerState) {
				//UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaughingSound, GetActorLocation());
				GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &ASquidgame_TestCharacter::Dead, 0.5f, false);
			}
		}
		else if (subtractVector == FVector(0, 0, 0))
		{
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Alive"));*/
		}

	}
	else if (!isDetecting)
	{
		originPos = currentPos;
		originRot = currentRot;
	}
}

void ASquidgame_TestCharacter::PrintInfoLog()
{
	FString gameModeString = GetWorld()->GetAuthGameMode() != nullptr ? *FString("Valid") : *FString("InValid");
	FString gameStateString = GetWorld()->GetGameState() != nullptr ? *FString("Valid") : *FString("InValid");
	FString playerStateString = GetPlayerState() != nullptr ? *FString("Valid") : *FString("InValid");
	FString printString = FString::Printf(TEXT("GameMode: %s \nGameState: %s\nPlayerState: %s\n"), *gameModeString, *gameStateString, *playerStateString);
	DrawDebugString(GetWorld(), GetActorLocation(), printString, nullptr, FColor::White, 0, true, 1.0f);
}


void ASquidgame_TestCharacter::Dead() {
	AController* MyController = GetController();
	if (MyController)
	{
		ANetRacePlayerState* MyPlayerState = Cast<ANetRacePlayerState>(MyController->PlayerState);
		if (MyPlayerState) {
			MyPlayerState->SetisDead();
			ServerDieProcess();
		}
	}
}

void ASquidgame_TestCharacter::ServerDieProcess_Implementation()
{
	MulticastDieProcess();
	//ForceNetUpdate();
}

void ASquidgame_TestCharacter::MulticastDieProcess_Implementation()
{
	if (!bAlive)
	{
		return;
	}

	bAlive = false;

	class APlayerController* pc = GetController<APlayerController>();
	if (pc != nullptr && pc->IsLocalPlayerController())
	{
		pc->SetIgnoreMoveInput(true);
		if (InGameUI)
		{
			InGameUI->ShowExitButtons();
		}
	}

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance)
	{
		animInstance->Montage_Play(DeadAnimMontage);
		UGameplayStatics::PlaySoundAtLocation(this, GunFiredSound, GetActorLocation(), 1);
	}
}

void ASquidgame_TestCharacter::ServerShowResult_Implementation()
{
	MulticastShowResult();
}

void ASquidgame_TestCharacter::MulticastShowResult_Implementation()
{
	class APlayerController* pc = GetController<APlayerController>();
	if (pc != nullptr && pc->IsLocalPlayerController())
	{
		if (InGameUI)
		{
			pc->SetShowMouseCursor(true);
			InGameUI->ShowGameResult();
		}
	}
}