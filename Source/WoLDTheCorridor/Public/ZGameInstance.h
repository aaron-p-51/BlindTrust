// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ZombieChaseTypes.h"
#include "ZGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WOLDTHECORRIDOR_API UZGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerType HostPlayerType;

	void SetHostPlayerType(EPlayerType PlayerType);

public:

	void SetPlayerType(int32 PlayerId, EPlayerType PlayerType);
	EPlayerType GetPlayerType(int32 PlayerId) const;

private:

	TMap<int32, EPlayerType> PlayerTypes;
	
	
	
};
