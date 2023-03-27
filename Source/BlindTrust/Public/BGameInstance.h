// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BlindTrustTypes.h"
#include "BGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API UBGameInstance : public UGameInstance
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerType DefaultHostPlayerType = EPlayerType::EPT_GuidePlayer;


public:

	
	void SetPlayerType(int32 PlayerId, EPlayerType PlayerType);

	EPlayerType GetPlayerType(int32 PlayerId) const;

	EPlayerType GetDefaultHostPlayerType() const { return DefaultHostPlayerType; }
	EPlayerType GetDefaultClientPlayerType() const
	{
		if (DefaultHostPlayerType == EPlayerType::EPT_BlindPlayer) return EPlayerType::EPT_GuidePlayer;
		else if (DefaultHostPlayerType == EPlayerType::EPT_GuidePlayer) return EPlayerType::EPT_BlindPlayer;
		else return EPlayerType::EPT_MAX;
	}

private:

	TMap<int32, EPlayerType> PlayerTypes;
	
	
	
};
