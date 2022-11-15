// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectMansionGameMode.h"
#include "ProjectMansionCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectMansionGameMode::AProjectMansionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
