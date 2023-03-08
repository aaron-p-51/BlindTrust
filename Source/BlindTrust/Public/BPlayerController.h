// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API ABPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void OnPossess(APawn* InPawn) override;
	
};
