// Fill out your copyright notice in the Description page of Project Settings.


#include "BGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"

#include "BGameInstance.h"

#include "BBlindPlayerCharacter.h"
#include "BGuidePlayerCharacter.h"
#include "BBlindPlayerSpawnVolume.h"
#include "BPlayerController.h"
#include "BZombie.h"

const FName BLIND_PLAYER_START = FName("BlindPlayerStart");
const FName GUIDE_PLAYER_START = FName("GuidePlayerStart");


namespace MatchState
{
	const FName ReplaceDefaultPawn = FName("ReplaceDefaultPawn");
	const FName Play = FName("Play");
}


ABGameMode::ABGameMode()
{
	bDelayedStart = true;
	bReplacedPawnForBlindPlayer = false;
	bReplacedPawnForGuidePlayer = false;
}


void ABGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin Play GameMode"));

	GetAllBlindPlayerStarts();
	GameInstance = Cast<UBGameInstance>(GetGameInstance());

}


void ABGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameInstance->GetDebugAllowEditorSinglePlayer())
	{
		if (MatchState == MatchState::WaitingToStart)
		{
			if (PlayersLogin > 0)
			{
				StartMatch();
			}
		}
		else if (MatchState == MatchState::ReplaceDefaultPawn)
		{
			if (bReplacedPawnForGuidePlayer || bReplacedPawnForBlindPlayer)
			{
				if (GameInstance->GetDebugSpawnZombieEditorSinglePlayer())
				{
					SpawnZombie();
				}

				SetMatchState(MatchState::Play);
			}
		}
	}
	else
	{
		if (MatchState == MatchState::WaitingToStart)
		{
			if (PlayersLogin == 2)
			{
				StartMatch();
			}
		}
		else if (MatchState == MatchState::ReplaceDefaultPawn)
		{
			if (bReplacedPawnForBlindPlayer && !bZombieSpawned)
			{
				SpawnZombie();
			}
			else if (bReplacedPawnForBlindPlayer && bReplacedPawnForGuidePlayer && bZombieSpawned)
			{
				UE_LOG(LogTemp, Warning, TEXT("Setting MatchState == MatchState::Play"));
				SetMatchState(MatchState::Play);
			}
		}
		else if (MatchState == MatchState::InProgress)
		{

		}
	}
}


void ABGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (!NewPlayer) return;

	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
	PlayersLogin++;

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

void ABGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	if (MatchState == MatchState::WaitingToStart)
	{
		//SetMatchState(MatchState::InProgress);
	}
	else if (MatchState == MatchState::InProgress)
	{
		SetMatchState(MatchState::ReplaceDefaultPawn);
	}
	else if (MatchState == MatchState::Play)
	{
		UE_LOG(LogTemp, Warning, TEXT("MatchState == MatchState::Play in OnMatchStateSet"));
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABPlayerController* BPlayerController = Cast<ABPlayerController>(*It);
		{
			if (BPlayerController)
			{
				BPlayerController->OnMatchStateSet(MatchState);
			}
		}
	}


}

void ABGameMode::ReplacePawnForPlayer(APlayerController* PlayerController, EPlayerType PlayerType)
{
	UE_LOG(LogTemp, Warning, TEXT("Replace player type: %d"), PlayerType);
	APawn* ControlledPawn = PlayerController->GetPawn();
	if (ControlledPawn)
	{
		ControlledPawn->Destroy();
	}

	if (PlayerType == EPlayerType::EPT_BlindPlayer)
	{
		

		FTransform BlindPlayerSpawnTransform;
		if (GetBlindPlayerStart(BlindPlayerSpawnTransform))
		{
			auto BlindCharacter = Cast<ABBlindPlayerCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BlindPlayerCharacterClass, BlindPlayerSpawnTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn));
			if (BlindCharacter)
			{
				UGameplayStatics::FinishSpawningActor(BlindCharacter, BlindPlayerSpawnTransform);
				PlayerController->Possess(Cast<APawn>(BlindCharacter));
				bReplacedPawnForBlindPlayer = true;
			}
		}
	}
	else if (PlayerType == EPlayerType::EPT_GuidePlayer)
	{
		GuidePlayerStart = GuidePlayerStart == nullptr ? GetGuidePlayerStart() : GuidePlayerStart;
		if (GuidePlayerStart == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Guide Player Start"));
			return;
		}

		auto GuideCharacter = Cast<ABGuidePlayerCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, GuidePlayerCharacterClass, GuidePlayerStart->GetActorTransform(), ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn));
		if (GuideCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawning Guide Player"));
			UGameplayStatics::FinishSpawningActor(GuideCharacter, GuidePlayerStart->GetActorTransform());
			PlayerController->Possess(Cast<APawn>(GuideCharacter));
			bReplacedPawnForGuidePlayer = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Spawning Guide Player"));
		}
	}
}

void ABGameMode::BlindPlayerCaught()
{
	SetMatchState(MatchState::WaitingPostMatch);
}

void ABGameMode::PlayerRequestToReturnToLobby(APlayerController* PlayerController)
{
	APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	if (PlayerState)
	{
		int32 PlayerId = PlayerState->GetPlayerId();
		RequestLeavePlayerIDs.Add(PlayerId);
	}

	if (RequestLeavePlayerIDs.Num() == 2)
	{
		GetWorld()->ServerTravel(FString("/Game/BlindTrust/Maps/Lobby?listen"));
	}
}

void ABGameMode::GetAllBlindPlayerStarts()
{
	BlindPlayerStarts.Empty();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ABBlindPlayerSpawnVolume::StaticClass(), FoundActors);

	if (FoundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ABBlindPlayerSpawnVolume Actors"));
	}

	for (const auto& FoundActor : FoundActors)
	{
		if (auto BlindPlayerSpawnVolume = Cast<ABBlindPlayerSpawnVolume>(FoundActor))
		{
			BlindPlayerStarts.Add(BlindPlayerSpawnVolume);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Found %d BlindPlayerSpawnVolumes"), BlindPlayerStarts.Num());

}

APlayerStart* ABGameMode::GetGuidePlayerStart() const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), FoundActors);

	for (const auto& FoundActor : FoundActors)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(FoundActor);
		if (PlayerStart && PlayerStart->ActorHasTag(GUIDE_PLAYER_START))
		{
			return PlayerStart;
		}
	}

	return nullptr;
}

bool ABGameMode::GetBlindPlayerStart(FTransform& StartTransform)
{
	if (BlindPlayerStarts.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No BlindPlayerSpawnVolues in map, unable to spawn BlindPlayer"));
		return false;
	}
	int32 Attempts = 0;
	while (Attempts < GetBlindPlayerStartAttempts)
	{
		ABBlindPlayerSpawnVolume* BlindPlayerSpawnVolume = BlindPlayerStarts[FMath::RandRange(0, BlindPlayerStarts.Num() - 1)];
		if (BlindPlayerSpawnVolume && BlindPlayerSpawnVolume->GetRandomSpawnPoint(StartTransform))
		{
			//SelectedBlindPlayerSpawnVolume = BlindPlayerSpawnVolume;
			BlindPlayerSpawnGroup = BlindPlayerSpawnVolume->GetSpawnGroup();
			return true;
		}

		++Attempts;
	}

	return false;
}

void ABGameMode::SpawnZombie()
{
	FTransform ZombieSpawnTransform;
	if (GetZombieStart(ZombieSpawnTransform))
	{
		ABZombie* Zombie = Cast<ABZombie>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ZombieCharacterClass, ZombieSpawnTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn));
		if (Zombie)
		{
			UGameplayStatics::FinishSpawningActor(Zombie, ZombieSpawnTransform);
			bZombieSpawned = true;
		}
	}
}

bool ABGameMode::GetZombieStart(FTransform& StartTransform)
{
	if (BlindPlayerStarts.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to spawn Zombie. Zombie will spawn in unoccupied BlindPlayerSpawnVolume. No BlindPlayerSpawnVolumes"));
		return false;
	}

	if (BlindPlayerStarts.Num() == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Only one BlindPlayerSapwnVolume in map. Zombie and BlindPlayer will spawn in same volume"));
		ABBlindPlayerSpawnVolume* SpawnVolume = BlindPlayerStarts[FMath::RandRange(0, BlindPlayerStarts.Num() - 1)];

		return SpawnVolume && SpawnVolume->GetRandomSpawnPoint(StartTransform);
	}

	int32 Attempts = 0;
	while (Attempts < GetBlindPlayerStartAttempts)
	{
		ABBlindPlayerSpawnVolume* SpawnVolume = BlindPlayerStarts[FMath::RandRange(0, BlindPlayerStarts.Num() - 1)];
		if (SpawnVolume && SpawnVolume->GetSpawnGroup() != BlindPlayerSpawnGroup && SpawnVolume->GetRandomSpawnPoint(StartTransform))
		{
			return true;	
		}

		++Attempts;
	}
	

	return false;
}

