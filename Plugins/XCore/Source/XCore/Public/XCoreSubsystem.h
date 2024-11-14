// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "XCoreSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class XCORE_API UXCoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UXCoreSubsystem();

protected:

private:

	IOnlineSessionPtr SessionInterface;
	
};
