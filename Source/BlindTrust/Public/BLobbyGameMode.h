// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlindTrustTypes.h"
#include "BLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API ABLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	void GetPlayerIds();

public:

	UFUNCTION(BlueprintCallable)
	void SetHostPlayerType(int32 HostPlayerId, EPlayerType PlayerType);

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
