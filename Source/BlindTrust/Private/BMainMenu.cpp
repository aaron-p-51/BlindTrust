// Fill out your copyright notice in the Description page of Project Settings.


#include "BMainMenu.h"

#include "Components/Button.h"
//#include "MultiplayerSessions/MultiplayerSessions/MultiplayerSessionsSubsystem.h" //"MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"

void UBMainMenu::MenuSetup(int32 NumberOfPublicConnections /*= 4*/, FString TypeOfMatch /*= FString(TEXT("FreeForAll"))*/, FString LobbyPath /*= FString(TEXT("/Game/ThirdPerson/Maps/Lobby?listen"))*/)
{

}

bool UBMainMenu::Initialize()
{
	Super::Initialize();

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
