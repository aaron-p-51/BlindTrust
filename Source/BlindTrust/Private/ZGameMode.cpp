// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "BGameInstance.h"

const FName BLIND_PLAYER_START = FName("BlindPlayerStart");
const FName GUIDE_PLAYER_START = FName("GuidePlayerStart");

void AZGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin Play"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), FoundActors);

	for (const auto& FoundActor : FoundActors)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(FoundActor);
		if (PlayerStart)
		{
			PlayerStarts.Add(PlayerStart);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Found %d player starts"), PlayerStarts.Num());

}

void AZGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (PlayerState)
	{
		if (UBGameInstance* BGameInstance = Cast<UBGameInstance>(GetGameInstance()))
		{
			EPlayerType PlayerType = BGameInstance->GetPlayerType(PlayerState->PlayerId);
			APlayerStart* PlayerStart = GetPlayerStartForPlayerType(PlayerType);
			if (PlayerStart)
			{
				RestartPlayerAtPlayerStart(NewPlayer, PlayerStart);
			}
		}
	}
}


APlayerStart* AZGameMode::GetPlayerStartForPlayerType(EPlayerType PlayerType)
{
	FName PlayerStartTag;

	if (PlayerType == EPlayerType::EPT_BlindPlayer)
	{
		PlayerStartTag = BLIND_PLAYER_START;
	}
	else if (PlayerType == EPlayerType::EPT_GuidePlayer)
	{
		PlayerStartTag = GUIDE_PLAYER_START;
	}

	for (const auto& PlayerStart : PlayerStarts)
	{
		if (PlayerStart->PlayerStartTag.IsEqual(PlayerStartTag))
		{
			return PlayerStart;
		}
	}

	return nullptr;
}