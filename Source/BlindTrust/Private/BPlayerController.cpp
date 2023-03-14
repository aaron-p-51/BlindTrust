// Fill out your copyright notice in the Description page of Project Settings.


#include "BPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

#include "BPlayerCharacter.h"
#include "BGameInstance.h"
#include "BGameMode.h"


#include "BBlindPlayerCharacter.h"
#include "BGuidePlayerCharacter.h"


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
			EPlayerType EPType = GetPlayerTypeFromGameInstance();
			if (EPType != EPlayerType::EPT_MAX)
			{
				if (auto GameMode = Cast<ABGameMode>(UGameplayStatics::GetGameMode(this)))
				{
					GameMode->ReplacePawnForPlayer(this, EPType);
					return;
				}
			}
		}

		if (ABGuidePlayerCharacter* GuidePlayerCharacter = Cast<ABGuidePlayerCharacter>(InPawn))
		{
			PlayerType = EPlayerType::EPT_GuidePlayer;
			OnRep_PlayerType();
		}
		else if (ABBlindPlayerCharacter* BlindPlayerCharacter = Cast<ABBlindPlayerCharacter>(InPawn))
		{
			PlayerType = EPlayerType::EPT_BlindPlayer;
			OnRep_PlayerType();
		}
	}
}

EPlayerType ABPlayerController::GetPlayerTypeFromGameInstance() const
{
	EPlayerType EPType = EPlayerType::EPT_MAX;

	if (auto GameInstance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (APlayerState* BPlayerState = GetPlayerState<APlayerState>())
		{
			EPType = GameInstance->GetPlayerType(BPlayerState->GetPlayerId());
		}
		if (EPType == EPlayerType::EPT_MAX)
		{
			// Is Server
			if (GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
			{
				EPType = GameInstance->GetDefaultHostPlayerType();
			}
			else if (GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
			{
				EPType = GameInstance->GetDefaultClientPlayerType();
			}
		}
	}

	return EPType;
}


void ABPlayerController::OnRep_PlayerType()
{
	if (IsLocalPlayerController())
	{
		OnLocalPlayerPlayerTypeChange(PlayerType);
	}
}





void ABPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABPlayerController, PlayerType);
}

