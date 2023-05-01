// Fill out your copyright notice in the Description page of Project Settings.


#include "BLobbyGameMode.h"

#include "GameFramework/PlayerState.h"
#include "BGameInstance.h"
#include "GameFramework/GameStateBase.h"

void ABLobbyGameMode::GetPlayerIds()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = Cast<APlayerController>(*It);
		int32 netId = PC->GetPlayerState<APlayerState>()->GetPlayerId();
		UE_LOG(LogTemp, Warning, TEXT("Player Unique ID: %d"), netId);


	}

}


void ABLobbyGameMode::SetHostPlayerType(int32 HostPlayerId, EPlayerType HostPlayerType)
{
	
	// Set ClientPlayerType to what ever HostPlayerType is not
	EPlayerType ClientPlayerType = HostPlayerType == EPlayerType::EPT_BlindPlayer ? EPlayerType::EPT_GuidePlayer : EPlayerType::EPT_BlindPlayer;
	int32 ClientPlayerId;
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = Cast<APlayerController>(*It);
		int32 netId = PC->GetPlayerState<APlayerState>()->GetPlayerId();
	
		if (netId != HostPlayerId)
		{
			ClientPlayerId = netId;
		}
	}
	
	if (UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance()))
	{
		GameInstance->SetPlayerType(HostPlayerId, HostPlayerType);
		GameInstance->SetPlayerType(ClientPlayerId, ClientPlayerType);
	}
}

void ABLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 1)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			//bUseSeamlessTravel = true;
			//World->ServerTravel(FString("/Game/Developers/AaronPetrek/Collections/Maps/DevTestingMap?listen"/*"/Game/Maps/BlasterMap?listen"*/));
			World->ServerTravel(FString("/Game/Maps/ZombieChase-01-BSP?listen"));
			/// Script / Engine.World'/Game/Developers/AaronPetrek/Collections/Maps/DevTestingMap.DevTestingMap'
		}
	}
}
