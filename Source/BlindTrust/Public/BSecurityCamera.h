// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSecurityCameraBase.h"
#include "BSecurityCamera.generated.h"

UCLASS()
class BLINDTRUST_API ABSecurityCamera : public ABSecurityCameraBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABSecurityCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Configuration
	 */
	 /** Min Camera pitch in local space. To avoid gimble lock value will be locked to -70 <= x <= 70 if set outside these values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Pitch", meta = (ClampMin = "-180.0", ClampMax = "180.0", UIMin = "-180.0", UIMax = "180.0"))
	float MinCameraPitchOffset;

	/** Max Camera pitch in local space. To avoid gimble lock value will be locked to -70 <= x <= 70 if set outside these values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Pitch", meta = (ClampMin = "-180.0", ClampMax = "180.0", UIMin = "-180.0", UIMax = "180.0"))
	float MaxCameraPitchOffset;

	/** Starting Pitch offset for camera. Will only be applied if set between Min and Max Camera Pitch Offset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Pitch")
	float StartingPitchOffset;

	/** Speed at which camera pitch will change after full input is applied. Higher values will change pitch faster. Default is 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Pitch", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float PitchMoveSpeed;

	/** Interpolation speed for full PitchMoveSpeed to be applied. Higher values will result in the pitch rotation rate reaching PitchMoveSpeed faster */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Pitch", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float PitchInterpSpeed;

	/** Min Camera Yaw in local space. To avoid gimble lock value will be locked to -70 <= x <= 70 if set outside these values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Yaw", meta = (ClampMin = "-180.0", ClampMax = "180.0", UIMin = "-180.0", UIMax = "180.0"))
	float MinCameraYawOffset;

	/** Max Camera Yaw in local space. To avoid gimble lock value will be locked to -70 <= x <= 70 if set outside these values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Yaw", meta = (ClampMin = "-180.0", ClampMax = "180.0", UIMin = "-180.0", UIMax = "180.0"))
	float MaxCameraYawOffset;

	/** Starting Yaw offset for camera. Will only be applied if set between Min and Max Camera Yaw Offset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Yaw")
	float StartingYawOffset;

	/** Speed at which camera yaw will change after full input is applied. Higher values will change pitch faster. Default is 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Yaw", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float YawMoveSpeed;

	/** Interpolation speed for full YawMoveSpeed to be applied. Higher values will result in the yaw rotation rate reaching YawMoveSpeed faster */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Yaw", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float YawInterpSpeed;


private:

	/** Pitch Input Members. Current Pitch input applied */
	float PitchInput;
	float CurrentInterpPitchInput;

	/** Yaw Input Members. Current Yaw input applied */
	float YawInput;
	float CurrentInterpYawInput;

	/** Current offset for yaw and rotation from initial rotation. Used to keep rotation clamped between min and max values */
	float CurrentYawOffset;
	float CurrentPitchOffset;

	/** Validate instance set members. Sets Calculated Pitch and Yaw member variables */
	void SetInitialValues();

	/** Add Pitch and Yaw to camera component */
	void ApplyPitchInput(float DeltaTime);
	void ApplyYawInput(float DeltaTime);

	/**
	 * Update Scene Caputre
	 */

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Input
	 */
	FORCEINLINE void AddPitch(float Value) { PitchInput = Value; }
	FORCEINLINE void AddYaw(float Value) { YawInput = Value; }


};
