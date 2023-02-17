// Fill out your copyright notice in the Description page of Project Settings.


#include "ZLobbyGameMode.h"
#include "GameFramework/PlayerState.h"
#include "ZGameInstance.h"

void AZLobbyGameMode::GetPlayerIds()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = Cast<APlayerController>(*It);
		int32 netId = PC->GetPlayerState<APlayerState>()->GetPlayerId();
		UE_LOG(LogTemp, Warning, TEXT("Player Unique ID: %d"), netId);


	}

}


void AZLobbyGameMode::SetHostPlayerType(int32 HostPlayerId, EPlayerType HostPlayerType)
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
	
	if (UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance()))
	{
		ZGameInstance->SetPlayerType(HostPlayerId, HostPlayerType);
		ZGameInstance->SetPlayerType(ClientPlayerId, ClientPlayerType);
	}
}