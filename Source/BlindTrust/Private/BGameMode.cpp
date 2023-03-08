// Fill out your copyright notice in the Description page of Project Settings.


#include "BGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "BGameInstance.h"

#include "BBlindPlayerCharacter.h"
#include "BGuidePlayerCharacter.h"

const FName BLIND_PLAYER_START = FName("BlindPlayerStart");
const FName GUIDE_PLAYER_START = FName("GuidePlayerStart");

void ABGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin Play"));

	

	UE_LOG(LogTemp, Warning, TEXT("Found %d player starts"), PlayerStarts.Num());

	GetAllPlayerStarts();

}

void ABGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (!NewPlayer) return;

	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));


	/*if (UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance()))
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance"));
		if (APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>())
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerState"));
			EPlayerType PlayerType = GameInstance->GetPlayerType(PlayerState->PlayerId);
			if (PlayerType != EPlayerType::EPT_MAX)
			{
				RestartPlayerAtPlayerStart(NewPlayer, GetPlayerStartForPlayerType(PlayerType));
			}
			else
			{
				APawn* Pawn = NewPlayer->GetPawn();
				if (!Pawn)
				{
					UE_LOG(LogTemp, Warning, TEXT("No Pawn yet"));
					return;
				}

				UE_LOG(LogTemp, Warning, TEXT("Remote Role: %d"), NewPlayer->GetPawn()->GetRemoteRole());
			}
		}
	}*/



	/*APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
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
	}*/
}


void ABGameMode::ReplacePawnForPlayer(APlayerController* PlayerController, EPlayerType PlayerType)
{
	UE_LOG(LogTemp, Warning, TEXT("Replace player type: %d"), PlayerType);
	APawn* ControlledPawn = PlayerController->GetPawn();
	if (ControlledPawn)
	{
		ControlledPawn->Destroy();
	}

	APlayerStart* NewPlayerStart = GetPlayerStartForPlayerType(PlayerType);
	FTransform SpawnTransform = NewPlayerStart->GetTransform();

	if (PlayerType == EPlayerType::EPT_BlindPlayer)
	{
		auto BlindCharacter = Cast<ABBlindPlayerCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BlindPlayerCharacterClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn));
		if (BlindCharacter)
		{
			UGameplayStatics::FinishSpawningActor(BlindCharacter, SpawnTransform);
		}

		PlayerController->Possess(Cast<APawn>(BlindCharacter));
	}
	else if (PlayerType == EPlayerType::EPT_GuidePlayer)
	{
		auto GuideCharacter = Cast<ABGuidePlayerCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, GuidePlayerCharacterClass, SpawnTransform,ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn));
		if (GuideCharacter)
		{
			UGameplayStatics::FinishSpawningActor(GuideCharacter, SpawnTransform);
		}

		PlayerController->Possess(Cast<APawn>(GuideCharacter));
	}
}


void ABGameMode::GetAllPlayerStarts()
{
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
}


APlayerStart* ABGameMode::GetPlayerStartForPlayerType(EPlayerType PlayerType)
{
	if (PlayerStarts.Num() == 0)
	{
		GetAllPlayerStarts();
	}

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