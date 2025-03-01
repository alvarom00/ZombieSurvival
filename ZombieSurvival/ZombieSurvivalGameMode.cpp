// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieSurvivalGameMode.h"
#include "ZombieSurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZombieSurvivalGameMode::AZombieSurvivalGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
