// Fill out your copyright notice in the Description page of Project Settings.


#include "XCoreSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"

UXCoreSubsystem::UXCoreSubsystem():
	OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(
		this, &ThisClass::OnCreateSessionComplete)),
	OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(
		this, &ThisClass::OnFindSessionComplete)),
	OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(
		this, &ThisClass::OnJoinSessionComplete)),
	OnDestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(
		this, &ThisClass::OnDestroySessionComplete)),
	OnStartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(
		this, &ThisClass::OnStartSessionComplete))
{
	
	// Get and validate the online subsystem.
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(UObject::GetWorld()))

		// Store a reference to the session interface.
		SessionInterface = Subsystem->GetSessionInterface();
	
}

void UXCoreSubsystem::CreateSession(const int32 NumPublicConnections, const FString& MatchType)
{

	if (!SessionInterface.IsValid()) return;

	// If a session currently exist, destroy it.
	if (SessionInterface->GetNamedSession(NAME_GameSession))

		SessionInterface->DestroySession(NAME_GameSession);

	// Create a handle for our delegate, so that 'OnCreateGameSessionCompleted' gets called after creating a game session.
	OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
		OnCreateSessionCompleteDelegate);

	// Get a reference to the hosting player.
	const ULocalPlayer* HostingPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	// Create and set the new session settings.
	SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = Online::GetSubsystem(UObject::GetWorld())->GetSubsystemName() == "NULL";
	SessionSettings->NumPublicConnections = NumPublicConnections;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// Create the session for the hosting player, with the specified settings.
	if (!SessionInterface->CreateSession(*HostingPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings))

		// If we fail to create a session, clear our delegate handle.
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	
}

void UXCoreSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{

	
	
}

void UXCoreSubsystem::FindSessions(const int32 MaxSearchResults)
{

	
	
}

void UXCoreSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{

	
	
}

void UXCoreSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{

	
	
}

void UXCoreSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	
	
}

void UXCoreSubsystem::DestroySession()
{

	
	
}

void UXCoreSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{

	
	
}

void UXCoreSubsystem::StartSession()
{

	
	
}

void UXCoreSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{

	
	
}
