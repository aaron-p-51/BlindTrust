// Fill out your copyright notice in the Description page of Project Settings.


#include "BPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"

#include "BPlayerCharacter.h"
#include "BGameInstance.h"
#include "BGameMode.h"


#include "BBlindPlayerCharacter.h"
#include "BGuidePlayerCharacter.h"
#include "BSecurityCameraController.h"


void ABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		SetInputMode(FInputModeGameOnly());

		UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance());
		if (GameInstance && GameInstance->bUseVsync)
		{
			GetWorld()->Exec(GetWorld(), TEXT("r.vsync 1"));
		}
	}

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

		// If EPType is EPT_MAX that we never set it, playing in th editor, use default player
		if (EPType == EPlayerType::EPT_MAX)
		{
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
		if (PlayerType == EPlayerType::EPT_GuidePlayer)
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(this, ABSecurityCameraController::StaticClass(), FoundActors);
			if (FoundActors.Num() == 1)
			{
				ABSecurityCameraController* FoundSecurityCameraController = Cast<ABSecurityCameraController>(FoundActors[0]);
				if (FoundSecurityCameraController)
				{
					FoundSecurityCameraController->SetAllRenderTargetsActive(true);
				}
			}
		}
	}
}


void ABPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;
	OnRep_MatchState();
}


void ABPlayerController::ReturnToLobby()
{
	if (GetLocalRole() != ENetRole::ROLE_Authority)
	{
		ServerReturnToLobby();
		return;
	}

	ABGameMode* BGameMode = Cast<ABGameMode>(GetWorld()->GetAuthGameMode());
	if (BGameMode)
	{
		BGameMode->PlayerRequestToReturnToLobby(this);
	}
}


void ABPlayerController::ServerReturnToLobby_Implementation()
{
	ReturnToLobby();
}


void ABPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::WaitingPostMatch)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blind Player Caught!!!"));

		if (IsLocalPlayerController())
		{
			ShowGameOver();
		}
	}
}


void ABPlayerController::ShowGameOver()
{
	if (!GameOverWidget)
	{
		GameOverWidget = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
	}
	if (GameOverWidget)
	{
		GameOverWidget->AddToViewport();
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void ABPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABPlayerController, PlayerType);
	DOREPLIFETIME(ABPlayerController, MatchState);
}




