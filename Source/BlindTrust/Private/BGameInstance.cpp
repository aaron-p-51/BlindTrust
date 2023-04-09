// Fill out your copyright notice in the Description page of Project Settings.


#include "BGameInstance.h"

void UBGameInstance::Init()
{
	Super::Init();

}

void UBGameInstance::SetHostPlayerType(EPlayerType PlayerType)
{

}




void UBGameInstance::SetPlayerType(int32 PlayerId, EPlayerType PlayerType)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting Player Type: %d"), PlayerId);
	PlayerTypes.FindOrAdd(PlayerId, PlayerType);
}


EPlayerType UBGameInstance::GetPlayerType(int32 PlayerId) const
{
	if (PlayerTypes.Contains(PlayerId))
	{
		return PlayerTypes[PlayerId];
	}

	return EPlayerType::EPT_MAX;
	//return EPlayerType::EPT_GuidePlayer;
}