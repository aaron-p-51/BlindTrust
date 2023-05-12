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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDebugAllowEditorSinglePlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bDebugAllowEditorSinglePlayer"))
	bool bDebugSpawnZombieEditorSinglePlayer = false;



public:

	void SetHostPlayerType(EPlayerType PlayerType);

	virtual void Init() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseVsync = true;


public:

	void SetPlayerType(int32 PlayerId, EPlayerType PlayerType);
	EPlayerType GetPlayerType(int32 PlayerId) const;

	UFUNCTION(BlueprintPure)
	EPlayerType GetDefaultHostPlayerType() const { return DefaultHostPlayerType; }

	UFUNCTION(BlueprintPure)
	EPlayerType GetDefaultClientPlayerType() const
	{
		if (DefaultHostPlayerType == EPlayerType::EPT_BlindPlayer) return EPlayerType::EPT_GuidePlayer;
		else if (DefaultHostPlayerType == EPlayerType::EPT_GuidePlayer) return EPlayerType::EPT_BlindPlayer;
		else return EPlayerType::EPT_MAX;
	}

	UFUNCTION(BlueprintPure)
	bool GetDebugAllowEditorSinglePlayer() const { return bDebugAllowEditorSinglePlayer; }

	UFUNCTION(BlueprintPure)
	bool GetDebugSpawnZombieEditorSinglePlayer() const { return bDebugSpawnZombieEditorSinglePlayer; }

private:

	TMap<int32, EPlayerType> PlayerTypes;
	
	
	
};
