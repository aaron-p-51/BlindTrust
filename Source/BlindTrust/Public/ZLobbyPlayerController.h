// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "ZLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API AZLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void TravelToGameMap();


	
};
