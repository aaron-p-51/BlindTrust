// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieChaseTypes.h"
#include "ZLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API AZLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	void GetPlayerIds();

public:

	UFUNCTION(BlueprintCallable)
	void SetHostPlayerType(int32 HostPlayerId, EPlayerType PlayerType);
	
};
