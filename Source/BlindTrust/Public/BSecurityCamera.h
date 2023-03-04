// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSecurityCameraBase.h"
#include "BSecurityCamera.generated.h"

UCLASS()
class BLINDTRUST_API ABSecurityCamera : public AZSecurityCameraBase
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Pitch", meta = (ClampMin = "-89.0", ClampMax = "89.0", UIMin = "-89.0", UIMax = "89.0"))
	float MinCameraPitch;

	/** Max Camera pitch in local space. To avoid gimble lock value will be locked to -70 <= x <= 70 if set outside these values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Pitch", meta = (ClampMin = "-89.0", ClampMax = "89.0", UIMin = "-89.0", UIMax = "89.0"))
	float MaxCameraPitch;

	/** Speed at which camera pitch will change after full input is applied. Higher values will change pitch faster. Default is 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Pitch", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float PitchMoveSpeed;

	/** Interpolation speed for full PitchMoveSpeed to be applied. Higher values will result in the pitch rotation rate reaching PitchMoveSpeed faster */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Pitch", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float PitchInterpSpeed;

	/** Min Camera Yaw in local space. To avoid gimble lock value will be locked to -70 <= x <= 70 if set outside these values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Yaw", meta = (ClampMin = "-89.0", ClampMax = "89.0", UIMin = "-89.0", UIMax = "89.0"))
	float MinCameraYaw;

	/** Max Camera Yaw in local space. To avoid gimble lock value will be locked to -70 <= x <= 70 if set outside these values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Yaw", meta = (ClampMin = "-89.0", ClampMax = "89.0", UIMin = "-89.0", UIMax = "89.0"))
	float MaxCameraYaw;

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

	/** Calculated values of valid pitch and yaw. Will set desired min and max pitch and yaw applied to starting camera rotation */
	float CalculatedMinPitch;
	float CalculatedMaxPitch;
	float CalculatedMinYaw;
	float CalculatedMaxYaw;
	
	/** Yaw Input Members. Current Yaw input applied */
	float YawInput;
	float CurrentInterpYawInput;

	/** Validate instance set members. Sets Calculated Pitch and Yaw member variables */
	void ValidateInstanceValues();

	/** Add Pitch and Yaw to camera component */
	void AddPitchInput(float DeltaTime);
	void AddYawInput(float DeltaTime);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	/**
	 * Input
	 */
	FORCEINLINE void AddPitch(float Value) { PitchInput = Value; }
	FORCEINLINE void AddYaw(float Value) { YawInput = Value; }

};
