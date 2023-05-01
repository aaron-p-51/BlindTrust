// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API UBMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/Lobby?listen")));


protected:

	virtual bool Initialize() override;

	// Removed in 5.1
	//virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	virtual void NativeDestruct() override;


	/**
	 * Callbacks for the custom delegates on the MultiplayerSessionSubsystem
	 */
	 UFUNCTION()
	 void OnCreateSession(bool bWasSuccessful);

	 void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);

	 void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

	 UFUNCTION()
	 void OnDestroySession(bool bWasSuccessful);

	 UFUNCTION()
	 void OnStartSession(bool bWasSuccessful);


private:

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();
	
	UFUNCTION()
	void JoinButtonClicked();

	void MenuTearDown();

	// The subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionSubsystem;

	int32 NumPublicConnections{4};

	FString MatchType{TEXT("FreeForAll")};

	FString PathToLobby{TEXT("")};
};
