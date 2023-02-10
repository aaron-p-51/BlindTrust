// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSecurityCameraController.generated.h"

UCLASS()
class WOLDTHECORRIDOR_API AZSecurityCameraController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZSecurityCameraController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* InteractionVolume;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MonitorScreen;

	UPROPERTY(EditDefaultsOnly)
	class USceneCaptureComponent2D* Camera;

	UPROPERTY(EditInstanceOnly)
	TArray<class AZSecurityCamera*> SecurityCameras;

	UPROPERTY()
	AZSecurityCamera* CurrentSecurityCamera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SwitchNextCamera();
	void SwitchPreviousCamera();

private:

	UPROPERTY()
	class UMaterialInstanceDynamic* MonitorScreenDynInstance;

	bool bIsSwitchingCameras;

	void SetInitialCamera();

	void MoveCameraToCurrentSecurityCamera();


};
