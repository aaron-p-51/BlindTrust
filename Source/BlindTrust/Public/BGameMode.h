// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlindTrustTypes.h"
#include "BGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API ABGameMode : public AGameMode
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;


	UPROPERTY()
	TArray<class ABBlindPlayerSpawnVolume*> BlindPlayerStarts;

	UPROPERTY(EditDefaultsOnly)
	int32 GetBlindPlayerStartAttempts = 100;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABBlindPlayerCharacter> BlindPlayerCharacterClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABGuidePlayerCharacter> GuidePlayerCharacterClass;

	UPROPERTY()
	class APlayerStart* GuidePlayerStart;

protected:


public:

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void ReplacePawnForPlayer(APlayerController* PlayerController, EPlayerType PlayerType);

private:


	APlayerStart* GetGuidePlayerStart() const;
	void GetAllBlindPlayerStarts();

	bool GetBlindPlayerStart(FTransform& StartTransform) const;
	
	

	
};
