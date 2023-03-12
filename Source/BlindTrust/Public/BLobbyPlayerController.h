// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlindTrustTypes.h"
#include "BLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API ABLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void TravelToGameMap();

	UFUNCTION(BlueprintCallable)
	void TravelToDevMap();

	UFUNCTION(BlueprintCallable)
	void SetPlayerType(EPlayerType PlayerType);

	
};
