// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XCoreMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class XCORE_API UXCoreMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	// The subsystem designed to handle all online session functionality.
	class UXCoreSubsystem* XCoreSubsystem;

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;
	
	int32 NumPublicConnections{3};
	
	FString MatchType{TEXT("AdventureMode")};

public:

	UFUNCTION(BlueprintCallable)
	void MenuSetup();

protected:

	virtual bool Initialize() override;

	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void SetNumPublicConnections(const int32 Amount);

	UFUNCTION(BlueprintCallable)
	void SetMatchType(const FString& Type);

private:

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();
	
};
