// Fill out your copyright notice in the Description page of Project Settings.


#include "BPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

#include "BPlayerCharacter.h"
#include "BGameInstance.h"
#include "BGameMode.h"

void ABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
}


void ABPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		auto BasePlayerCharacter = Cast<ABPlayerCharacter>(InPawn);
		if (!BasePlayerCharacter)
		{
			EPlayerType PlayerType = GetPlayerTypeFromGameInstance();
			if (PlayerType != EPlayerType::EPT_MAX)
			{
				if (auto GameMode = Cast<ABGameMode>(UGameplayStatics::GetGameMode(this)))
				{
					GameMode->ReplacePawnForPlayer(this, PlayerType);
				}
			}
		}
	}
}

EPlayerType ABPlayerController::GetPlayerTypeFromGameInstance() const
{
	EPlayerType PlayerType = EPlayerType::EPT_MAX;

	if (auto GameInstance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (APlayerState* BPlayerState = GetPlayerState<APlayerState>())
		{
			PlayerType = GameInstance->GetPlayerType(BPlayerState->GetPlayerId());
		}
		if (PlayerType == EPlayerType::EPT_MAX)
		{
			// Is Server
			if (GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
			{
				PlayerType = GameInstance->GetDefaultHostPlayerType();
			}
			else if (GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
			{
				PlayerType = GameInstance->GetDefaultClientPlayerType();
			}
		}
	}

	return PlayerType;
}
