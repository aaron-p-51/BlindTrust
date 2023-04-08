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

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnMatchStateSet(FName State);

protected:

	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;


	EPlayerType GetPlayerTypeFromGameInstance() const;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerType, BlueprintReadWrite, VisibleAnywhere)
	EPlayerType PlayerType;

	UFUNCTION()
	void OnRep_PlayerType();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLocalPlayerPlayerTypeChange(EPlayerType Type);

	/**
	 * Config
	 */
	UPROPERTY(VisibleAnywhere, Category = "Configuration")
	UUserWidget* GameOverWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	TSubclassOf<UUserWidget> GameOverWidgetClass;


private:

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	void ShowGameOver();
};
