// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BBlindPlayerSpawnVolume.generated.h"

UCLASS()
class BLINDTRUST_API ABBlindPlayerSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABBlindPlayerSpawnVolume();

protected:

	/** Volume where character can spawn. Volume must intersect the floor */
	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* SpawnVolume;

	/**
	 * Offset from floor (+Z direction) where to spawn character inside SpawnVolume. ie distance above floor
	 * TODO: Remove and add argument to GetRandomSpawnPoint()
	 */
	UPROPERTY(EditAnywhere, Category = "Config")
	float SpawnZAxisOffset = 92.f;

	/** Specify Spawn group, Zombie will not spawn in same group as BlindPlayer */
	UPROPERTY(EditInstanceOnly, Category = "Config")
	int32 SpawnGroup = 0;

public:	

	/** Get a random FTransform inside of SpawnVolume */
	bool GetRandomSpawnPoint(FTransform& SpawnTransform) const;

	FORCEINLINE int32 GetSpawnGroup() const { return SpawnGroup; }

private:
	
	/** Get a random yaw rotation (0, 360) */
	FQuat GetRandomSpawnRotation() const;

};
