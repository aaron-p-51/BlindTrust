// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ZombieChaseTypes.h"
#include "ZGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API AZGameMode : public AGameMode
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<class APlayerStart*> PlayerStarts;

protected:

	APlayerStart* GetPlayerStartForPlayerType(EPlayerType PlayerType);

public:

	virtual void PostLogin(APlayerController* NewPlayer) override;


	

	
};
