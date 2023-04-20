// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BBlindTrustGameState.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API ABBlindTrustGameState : public AGameState
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(Replicated)
	float StartChaseTime = 0.f;

	UPROPERTY(Replicated)
	float BlindPlayerCaughtTime = 0.f;

public:

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetStartChaseTime() const { return StartChaseTime; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetBlindPlayerCaughtTime() const { return BlindPlayerCaughtTime; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetSurvivalTimeDuration() const { return BlindPlayerCaughtTime - StartChaseTime; }

	void SetStartChaseTime();
	void SetBlindPlayerCaughtTime();


	
};
