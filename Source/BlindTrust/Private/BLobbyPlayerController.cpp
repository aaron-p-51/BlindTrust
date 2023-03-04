// Fill out your copyright notice in the Description page of Project Settings.


#include "BLobbyPlayerController.h"


#include "GameFramework/PlayerState.h"

void ABLobbyPlayerController::TravelToGameMap()
{
	GetWorld()->ServerTravel(FString("/Game/ZombieChase/Maps/LANTestLevel?listen"));
}


