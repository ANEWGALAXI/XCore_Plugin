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
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SearchResult);
	void DestroySession();
	void StartSession();

protected:

	//
	//	Internal callbacks for the delegates that we'll add to the Online Session Interface delegate list.
	//	These don't need to be called outside this class.
	//
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnFindSessionCompleted(bool bWasSuccessful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnStartSessionCompleted(FName SessionName, bool bWasSuccessful);

private:
	
	IOnlineSessionPtr SessionInterface;

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
