// Fill out your copyright notice in the Description page of Project Settings.


#include "BMainMenu.h"

#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h" //"MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"

void UBMainMenu::MenuSetup(int32 NumberOfPublicConnections /*= 4*/, FString TypeOfMatch /*= FString(TEXT("FreeForAll"))*/, FString LobbyPath /*= FString(TEXT("/Game/ThirdPerson/Maps/Lobby?listen"))*/)
{
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);

			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &UBMainMenu::OnCreateSession);
		MultiplayerSessionSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &UBMainMenu::OnFindSessions);
		MultiplayerSessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &UBMainMenu::OnJoinSession);
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UBMainMenu::OnDestroySession);
		MultiplayerSessionSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &UBMainMenu::OnStartSession);
	}
}

bool UBMainMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UBMainMenu::HostButtonClicked);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UBMainMenu::JoinButtonClicked);
	}

	return true;
}

void UBMainMenu::NativeDestruct()
{
	Super::NativeDestruct();

	MenuTearDown();
}



void UBMainMenu::OnCreateSession(bool bWasSuccessful)
{

}

void UBMainMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful)
{

}

void UBMainMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{

}

void UBMainMenu::OnDestroySession(bool bWasSuccessful)
{

}

void UBMainMenu::OnStartSession(bool bWasSuccessful)
{

}

void UBMainMenu::HostButtonClicked()
{

}

void UBMainMenu::JoinButtonClicked()
{

}

void UBMainMenu::MenuTearDown()
{

}
