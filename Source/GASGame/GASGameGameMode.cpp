// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASGameGameMode.h"
#include "GASGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGASGameGameMode::AGASGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
