// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSecurityCamera.generated.h"

UCLASS()
class WOLDTHECORRIDOR_API AZSecurityCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZSecurityCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MonitorScreen;

	UPROPERTY(EditDefaultsOnly)
	class USceneCaptureComponent2D* Camera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetCameraLocationAndRotation(FVector& Location, FRotator& Rotation);


};
