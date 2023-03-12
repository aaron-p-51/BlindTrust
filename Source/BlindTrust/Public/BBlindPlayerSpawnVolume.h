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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere)
	float SpawnZAxisOffset = 92.f;

public:	

	bool GetRandomSpawnPoint(FTransform& SpawnTransform) const;

private:
	
	FQuat GetRandomSpawnRotation() const;

};
