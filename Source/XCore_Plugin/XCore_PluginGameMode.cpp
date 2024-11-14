// Copyright Epic Games, Inc. All Rights Reserved.

#include "XCore_PluginGameMode.h"
#include "XCore_PluginCharacter.h"
#include "UObject/ConstructorHelpers.h"

AXCore_PluginGameMode::AXCore_PluginGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
