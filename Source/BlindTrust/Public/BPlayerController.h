// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlindTrustTypes.h"
#include "BPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API ABPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;


	EPlayerType GetPlayerTypeFromGameInstance() const;
	
};
