// Fill out your copyright notice in the Description page of Project Settings.


#include "XCoreSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

UXCoreSubsystem::UXCoreSubsystem():
	OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(
		this, &ThisClass::OnCreateSessionCompleted)),
	OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(
		this, &ThisClass::OnFindSessionCompleted)),
	OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(
		this, &ThisClass::OnJoinSessionCompleted)),
	OnDestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(
		this, &ThisClass::OnDestroySessionCompleted)),
	OnStartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(
		this, &ThisClass::OnStartSessionCompleted))
{
	
	// Get and validate the online subsystem.
	if (IOnlineSubsystem* Subsystem = Online::GetSubsystem(UObject::GetWorld()); Subsystem)

		// Store a reference to the session interface.
		SessionInterface = Subsystem->GetSessionInterface();
	
}

void UXCoreSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{

	
	
}

void UXCoreSubsystem::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{

	
	
}

void UXCoreSubsystem::FindSessions(int32 MaxSearchResults)
{

	
	
}

void UXCoreSubsystem::OnFindSessionCompleted(bool bWasSuccessful)
{

	
	
}

void UXCoreSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{

	
	
}

void UXCoreSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	
	
}

void UXCoreSubsystem::DestroySession()
{

	
	
}

void UXCoreSubsystem::OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful)
{

	
	
}

void UXCoreSubsystem::StartSession()
{

	
	
}

void UXCoreSubsystem::OnStartSessionCompleted(FName SessionName, bool bWasSuccessful)
{

	
	
}
