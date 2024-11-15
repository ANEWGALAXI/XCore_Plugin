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

	//
	// To handle session functionality.
	//
	void CreateSession(const int32 NumPublicConnections, const FString& MatchType);
	void FindSessions(const int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SearchResult);
	void DestroySession();
	void StartSession();

protected:

	//
	//	Internal callbacks for the delegates that we'll add to the Online Session Interface delegate list.
	//	These don't need to be called outside this class.
	//
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSettings> SessionSettings;

	//
	//	To add to the Online Session Interface delegate list.
	//	We'll bind our XCoreSession internal callbacks to these.
	//
	FOnCreateSessionCompleteDelegate	OnCreateSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate		OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate		OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate	OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FOnStartSessionCompleteDelegate		OnStartSessionCompleteDelegate;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	
};
