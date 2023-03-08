// Fill out your copyright notice in the Description page of Project Settings.


#include "BPlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "BPlayerCharacter.h"
#include "BGameInstance.h"
#include "BGameMode.h"

void ABPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		auto BasePlayerCharacter = Cast<ABPlayerCharacter>(InPawn);
		if (!BasePlayerCharacter)
		{
			UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance());
			if (GameInstance)
			{
				//ENetRole RemoteRole = GetRemoteRole();
				EPlayerType PlayerType = EPlayerType::EPT_MAX;
				ENetRole PlayerRemoteRole = GetRemoteRole();

				// Is Server
				if (PlayerRemoteRole == ENetRole::ROLE_SimulatedProxy)
				{
					PlayerType = GameInstance->GetDefaultHostPlayerType();
				}
				else if (PlayerRemoteRole == ENetRole::ROLE_AutonomousProxy)
				{
					PlayerType = GameInstance->GetDefaultClientPlayerType();
				}
				else
				{
					return;
				}

				if (PlayerType == EPlayerType::EPT_MAX) return;
				auto GameMode = Cast<ABGameMode>(UGameplayStatics::GetGameMode(this));

				if (GameMode)
				{
					GameMode->ReplacePawnForPlayer(this, PlayerType);
				}

			}
		}
	}
	
	
}