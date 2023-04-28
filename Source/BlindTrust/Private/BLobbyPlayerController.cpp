// Fill out your copyright notice in the Description page of Project Settings.


#include "BLobbyPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

#include "BLobbyGameMode.h"

void ABLobbyPlayerController::TravelToGameMap()
{
	GetWorld()->ServerTravel(FString("/Game/Maps/ZombieChase-01-BSP?listen"));
}


void ABLobbyPlayerController::TravelToDevMap()
{
	GetWorld()->ServerTravel(FString("/Game/Developers/AaronPetrek/Collections/Maps/DevTestingMap?listen"));
}


void ABLobbyPlayerController::SetPlayerType(EPlayerType PlayerType)
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		int32 PlayerId = GetPlayerState<APlayerState>()->GetPlayerId();
		
		if (auto GameMode = Cast<ABLobbyGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->SetHostPlayerType(PlayerId, PlayerType);
		}
	}
}

