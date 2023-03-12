// Fill out your copyright notice in the Description page of Project Settings.


#include "BGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "BGameInstance.h"

#include "BBlindPlayerCharacter.h"
#include "BGuidePlayerCharacter.h"
#include "BBlindPlayerSpawnVolume.h"

const FName BLIND_PLAYER_START = FName("BlindPlayerStart");
const FName GUIDE_PLAYER_START = FName("GuidePlayerStart");

void ABGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin Play GameMode"));

	//GuidePlayerStart = GetGuidePlayerStart();
	//GetAllBlindPlayerStarts();


	//UE_LOG(LogTemp, Warning, TEXT("Found %d player starts"), PlayerStarts.Num());

	//GuidePlayerStartTransform = GetGuidePlayerStart();

	//GetAllPlayerStarts();

	//GetAllBlindPlayerStarts();

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

	//APlayerStart* NewPlayerStart = GetPlayerStartForPlayerType(PlayerType);
	//FTransform SpawnTransform = NewPlayerStart->GetTransform();

	if (PlayerType == EPlayerType::EPT_BlindPlayer)
	{
		GetAllBlindPlayerStarts();

		FTransform BlindPlayerSpawnTransform;
		if (GetBlindPlayerStart(BlindPlayerSpawnTransform))
		{
			
			auto BlindCharacter = Cast<ABBlindPlayerCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BlindPlayerCharacterClass, BlindPlayerSpawnTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn));
			if (BlindCharacter)
			{
				UGameplayStatics::FinishSpawningActor(BlindCharacter, BlindPlayerSpawnTransform);
				PlayerController->Possess(Cast<APawn>(BlindCharacter));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Blind Player Start"));
		}


		/*auto BlindCharacter = Cast<ABBlindPlayerCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BlindPlayerCharacterClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn));
		if (BlindCharacter)
		{

			if (GetBlindPlayerStart(BlindPlayerSpawnTransform))
			{
				UGameplayStatics::FinishSpawningActor(BlindCharacter, BlindPlayerSpawnTransform);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Unable to find spawn point for Blind Player"));
			}
		}*/

		
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
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Spawning Guide Player"));
		}
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


bool ABGameMode::GetBlindPlayerStart(FTransform& StartTransform) const
{


	int32 Attempts = 0;
	while (Attempts < GetBlindPlayerStartAttempts)
	{
		if (ABBlindPlayerSpawnVolume* BlindPlayerSpawnVolume = BlindPlayerStarts[FMath::RandRange(0, BlindPlayerStarts.Num() - 1)])
		{
			if (BlindPlayerSpawnVolume->GetRandomSpawnPoint(StartTransform))
			{
				return true;
			}
		}

		++Attempts;
	}

	return false;
}

