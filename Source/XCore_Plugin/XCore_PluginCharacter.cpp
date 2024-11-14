// Copyright Epic Games, Inc. All Rights Reserved.

#include "XCore_PluginCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AXCore_PluginCharacter

AXCore_PluginCharacter::AXCore_PluginCharacter():
	// Bind the respective functions to our delegates
	OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(
		this, &ThisClass::OnCreateGameSessionComplete)),
	OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(
		this, &ThisClass::OnFindSessionsComplete)),
	OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(
		this, &ThisClass::OnJoinSessionComplete))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Get an instance of the online subsystem.
	if (IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()); OnlineSubsystem != nullptr)
	{

		// Store a reference to the session interface.
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		if (GEngine)
		{

			// Print a debug message to the screen.
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
				FString::Printf(TEXT("Found online subsystem: %s"),
					*OnlineSubsystem->GetSubsystemName().ToString()));
			
		}
			
	}
	
}

void AXCore_PluginCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AXCore_PluginCharacter::CreateGameSession()
{
	
	if (!OnlineSessionInterface.IsValid()) return;

	// If a session already exists.
	if (OnlineSessionInterface->GetNamedSession(NAME_GameSession) != nullptr)
	{
		// Destroy it.
		OnlineSessionInterface->DestroySession(NAME_GameSession);		
		
	}

	// Add a handle for our delegate, so that 'OnCreateGameSessionComplete' gets called after creating a game session.
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	// Get the hosting player's id.
	const FUniqueNetIdRepl HostingPlayerId = GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId();

	// Create and set the new session settings.
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = false;
	SessionSettings->NumPublicConnections = 3;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->Set(FName("MatchType"), FString("AdventureMode"),
		EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// Create the session
	OnlineSessionInterface->CreateSession(*HostingPlayerId, NAME_GameSession, *SessionSettings);
	
}

void AXCore_PluginCharacter::OnCreateGameSessionComplete(FName SessionName, bool bWasSuccessful)
{

	// If the session was created successfully
	if (bWasSuccessful)
	{

		if (GEngine)
		{

			// Print a debug message to the screen.
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
				FString::Printf(TEXT("Created session: %s"),
					*SessionName.ToString()));
			
		}

		// Get and validate the current world.
		if (UWorld* World = GetWorld())
		{

			// Travel to the new map in the current world.
			World->ServerTravel(FString("/Game/XCore_Plugin/Maps/LobbyMap?listen"));
			
		}
		
	}
	else
	{

		// Print a debug message to the screen.
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
			FString::Printf(TEXT("Failed to create session: %s"), *SessionName.ToString()));
		
	}
	
}

void AXCore_PluginCharacter::JoinGameSession()
{

	// Validate the online session interface.
	if (!OnlineSessionInterface.IsValid()) return;

	// Add a handle for our delegate so that 'OnFindSessionsComplete' gets called after searching for sessions.
	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	// Create and set the new session search settings.
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(FName(TEXT("PRESENCESEARCH")), true, EOnlineComparisonOp::Equals);

	// Get the hosting player's id.
	const FUniqueNetIdRepl HostingPlayerId = GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId();

	// Find any available sessions that match our settings.
	OnlineSessionInterface->FindSessions(*HostingPlayerId, SessionSearch.ToSharedRef());
	
}

void AXCore_PluginCharacter::OnFindSessionsComplete(bool bWasSuccessful)
{

	// Validate the online session interface.
	if (!OnlineSessionInterface.IsValid()) return;

	if (bWasSuccessful)
	{

		for (auto Result : SessionSearch->SearchResults)
		{

			auto SessionId = Result.GetSessionIdStr();
			auto User = Result.Session.OwningUserName;

			FString MatchType;
			Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

			if (MatchType == FString("AdventureMode"))
			{

				if (GEngine)
				{

					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
					FString::Printf(TEXT("SessionId: %s, User: %s, MatchType: %s"),
						*SessionId, *User, *MatchType));

					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow,
						FString::Printf(TEXT("Joining session: %s"), *SessionId));
				
				}

				// Add a handle for our delegate so that 'OnJoinSessionComplete' gets called after joining a session.
				OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

				// Get the joining player's id.
				const FUniqueNetIdRepl JoiningPlayerId =
					GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId();

				// Join the current session.
				OnlineSessionInterface->JoinSession(*JoiningPlayerId, NAME_GameSession, Result);
				
			}
		
		}
		
	}
	
}

void AXCore_PluginCharacter::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	// Validate the online session interface.
	if (!OnlineSessionInterface.IsValid()) return;

	// If we successfully connected to a session.
	if (FString Address; OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
			FString::Printf(TEXT("Connect string: %s"), *Address));
		}

		// Get the first local PlayerController.
		if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
		{

			// Travel the client to the address.
			PlayerController->ClientTravel(Address, TRAVEL_Absolute);
			
		}
		
	}
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AXCore_PluginCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AXCore_PluginCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AXCore_PluginCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AXCore_PluginCharacter::Move(const FInputActionValue& Value)
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

void AXCore_PluginCharacter::Look(const FInputActionValue& Value)
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