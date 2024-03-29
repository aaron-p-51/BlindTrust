// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSecurityCameraBase.h"
#include "BSecurityCameraController.generated.h"

UCLASS()
class BLINDTRUST_API ABSecurityCameraController : public ABSecurityCameraBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABSecurityCameraController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Components
	 */

	/** Interaction volume for player to use SecurityCameraController. */
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* InteractionVolume;

	/**
	 * Config
	 */
	/** Instance set of all AZSecurityCamera's for this ZSecurityCameraController will control */
	UPROPERTY(EditInstanceOnly)
	TArray<class ABSecurityCamera*> SecurityCameras;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float CameraSwitchDelay;


	/**
	 * Status
	 */
	/** Current AZSecurityCamera in SecurityCameras under control */
	UPROPERTY()
	ABSecurityCamera* CurrentSecurityCamera;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Config
	 */
	void SetAllRenderTargetsActive(bool Value);

	/**
	 * Input
	 */
	/** Input, switch camera in SecurityCameras */
	void SwitchNextCamera();
	void SwitchPreviousCamera();

	/** Add Pitch input to CurrentSecurityCamera */
	void AddCurrentCameraPitch(float Value);

	/** Add Yaw input to CurrentSecurityCamera */
	void AddCurrentCameraYaw(float Value);


private:

	/** Inform Player Pawn they are overlapping InteractionVolume */
	UFUNCTION()
	void OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Inform Player Pawn they are no longer overlapping InteractionVolume */
	UFUNCTION()
	void OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Set camera component position and rotation to first index of SecurityCameras camera */
	void SetInitialCamera();

	/** Move Camera component to current position and rotation of camera in CurrentSecurityCamera */
	void MoveCameraToCurrentSecurityCamera();


	/**
	 * Switching Cameras
	 */

	/** Currently switching camera, do not allow camera switching while currently in the process of camera switching. ie camera switching in blocking */
	bool bIsSwitchingCameras;

	/** Timer handle for switching cameras, duration specified by CameraSwitchDelay. Will call SwitchCameraDelayComplete when time value expires  */
	FTimerHandle TimerHandle_SwitchCameraDelay;

	/** Complete process of switching cameras. Called with no delay if CameraSwitchDelay is 0 */
	UFUNCTION()
	void SwitchCameraDelayComplete();

	/** Show screen static effect on Monitor Screen. */
	void ShowStaticMonitorScreen(bool Value);

};
