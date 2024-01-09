// Copyright Epic Games, Inc. All Rights Reserved.

#include "Squidgame_TestGameMode.h"
#include "Squidgame_TestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASquidgame_TestGameMode::ASquidgame_TestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
