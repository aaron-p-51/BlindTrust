// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPlayerCharacter.h"
#include "BBlindPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API ABBlindPlayerCharacter : public ABPlayerCharacter
{
	GENERATED_BODY()

public:

	ABBlindPlayerCharacter();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> BlindOverlayWidgetClass;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* BlindOverlayWidget;
	
};
