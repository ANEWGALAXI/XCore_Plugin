// Fill out your copyright notice in the Description page of Project Settings.


#include "XCoreMenu.h"

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
	
}
