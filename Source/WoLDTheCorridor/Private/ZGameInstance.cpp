// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameInstance.h"

void UZGameInstance::SetHostPlayerType(EPlayerType PlayerType)
{

}


void UZGameInstance::SetPlayerType(int32 PlayerId, EPlayerType PlayerType)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting Player Type: %d"), PlayerId);
	PlayerTypes.FindOrAdd(PlayerId, PlayerType);
}


EPlayerType UZGameInstance::GetPlayerType(int32 PlayerId) const
{
	if (PlayerTypes.Contains(PlayerId))
	{
		return PlayerTypes[PlayerId];
	}

	return EPlayerType::EPT_MAX;
}