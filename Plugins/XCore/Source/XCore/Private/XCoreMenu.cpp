// Fill out your copyright notice in the Description page of Project Settings.


#include "XCoreMenu.h"
#include "XCoreSubsystem.h"
#include "Components/Button.h"

void UXCoreMenu::MenuSetup()
{
	
	AddToViewport();
	SetIsFocusable(true);
	SetVisibility(ESlateVisibility::Visible);

	if (const UWorld* World = GetWorld())
	{
		
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{

			// Create and set the input mode settings.
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			// Apply the input mode settings.
			PlayerController->SetInputMode(InputModeData);

			// Make sure to display the mouse cursor.
			PlayerController->SetShowMouseCursor(true);
			
		}
		
	}

	// Get and validate a reference of the game instance.
	if (const UGameInstance* GameInstance = GetGameInstance())
	{

		// Retrieve a reference to our custom subsystem from the game instance.
		XCoreSubsystem = GameInstance->GetSubsystem<UXCoreSubsystem>();
		if (XCoreSubsystem)
			// Bind our callback to the respective delegate so that 'OnCreateSession' gets called after a session is created.
			XCoreSubsystem->XCoreOnCreateSessionCompleteDelegate.AddDynamic(this, &ThisClass::OnCreateSession);
		
	}
	
}

bool UXCoreMenu::Initialize()
{

	// Return false if we fail to initialize
	if (!Super::Initialize()) return false;
	
	if (HostButton != nullptr && JoinButton != nullptr)
	{
		// Bind our respective functions to the OnClicked delegate.
		
		HostButton->OnClicked.AddDynamic(this, &ThisClass::UXCoreMenu::HostButtonClicked);

		JoinButton->OnClicked.AddDynamic(this, &ThisClass::UXCoreMenu::JoinButtonClicked);
		
	} else

		return false;
	
	return true;
	
}

void UXCoreMenu::HostButtonClicked()
{

	if (GEngine)

		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Hosting session...");

	if (XCoreSubsystem)
	{
		
		XCoreSubsystem->CreateSession(NumPublicConnections, MatchType);
		
	}
	
}

void UXCoreMenu::OnCreateSession(bool bWasSuccessful)
{

	if (bWasSuccessful)
	{
		
		if (GEngine)

			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Session created successfully.");

		if (UWorld* World = GetWorld())

			World->ServerTravel("/Game/XCore_Plugin/Maps/LobbyMap?listen");
		
	}
	else
	{

		if (GEngine)

			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Session creation failed!");
		
	}
	
}

void UXCoreMenu::JoinButtonClicked()
{

	if (GEngine)

		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Joining session...");
	
}

void UXCoreMenu::NativeDestruct()
{

	RemoveFromParent();

	if (const UWorld* World = GetWorld())

		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			
			const FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
			
		}
	
	Super::NativeDestruct();
	
}

void UXCoreMenu::SetNumPublicConnections(const int32 Amount)
{

	NumPublicConnections = Amount;
	
}

void UXCoreMenu::SetMatchType(const FString& Type)
{

	MatchType = Type;
	
}
