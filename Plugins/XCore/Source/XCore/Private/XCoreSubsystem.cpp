// Fill out your copyright notice in the Description page of Project Settings.


#include "XCoreSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

UXCoreSubsystem::UXCoreSubsystem()
{
	// Get and validate the online subsystem.
	if (IOnlineSubsystem* Subsystem = Online::GetSubsystem(UObject::GetWorld()); Subsystem)

		// Store a reference to the session interface.
		SessionInterface = Subsystem->GetSessionInterface();
	
}
