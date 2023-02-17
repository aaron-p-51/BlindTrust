// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyPlayerController.h"


#include "GameFramework/PlayerState.h"

void AZLobbyPlayerController::TravelToGameMap()
{
	GetWorld()->ServerTravel(FString("/Game/ZombieChase/Maps/TestGameLevel?listen"));
}


