// Fill out your copyright notice in the Description page of Project Settings.


#include "BSecurityCamera.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Materials/MaterialInstanceDynamic.h"

const FName VFX_LINE_SPEED_PARAM = FName("VFXLineSpeed");

// Sets default values
ABSecurityCamera::ABSecurityCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinCameraPitch = -70.f;
	MaxCameraPitch = 70.f;
	PitchMoveSpeed = 1.f;
	PitchInterpSpeed = 0.4f;

	MinCameraYaw = -70.f;
	MaxCameraYaw = 70.f;
	YawMoveSpeed = 1.f;
	YawInterpSpeed = 0.4f;
}


// Called when the game starts or when spawned
void ABSecurityCamera::BeginPlay()
{
	Super::BeginPlay();

	ValidateInstanceValues();
}


void ABSecurityCamera::ValidateInstanceValues()
{
	// Validate Pitch values set for instances of this class
	if (MinCameraPitch < -89.f || MinCameraPitch > 89.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has invalid MinCameraPitch: %f, will set to -89.0"), *GetName(), MinCameraPitch);
		MinCameraPitch = -89.f;
	}
	if (MaxCameraPitch < -89.f || MaxCameraPitch > 89.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has invalid MaxCameraPitch: %f, will set to 89.0"), *GetName(), MaxCameraPitch);
		MaxCameraPitch = -89.f;
	}
	if (MinCameraPitch > MaxCameraPitch)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s MinCameraPitch is greater than MaxCameraPitch"), *GetName());
	}

	// Validate yaw values set for instances of this class
	if (MinCameraYaw < -89.f || MinCameraYaw > 89.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has invalid MinCameraYaw: %f, will set -89.0"), *GetName(), MinCameraYaw);
		MinCameraYaw = -89.f;
	}
	if (MaxCameraYaw < -89.f || MaxCameraYaw > 89.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has invalid MaxCameraYaw: %f, will set 89.0"), *GetName(), MaxCameraYaw);
		MaxCameraYaw = -89.f;
	}
	if (MinCameraYaw > MaxCameraYaw)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s MinCameraYaw is greater than MaxCameraYaw"), *GetName());
	}

	if (Camera)
	{
		FRotator StartingCameraRotation = Camera->GetComponentRotation();
		CalculatedMinYaw = StartingCameraRotation.Yaw - FMath::Abs(MinCameraYaw);
		CalculatedMaxYaw = StartingCameraRotation.Yaw + FMath::Abs(MaxCameraYaw);

		CalculatedMinPitch = StartingCameraRotation.Pitch - FMath::Abs(MinCameraPitch);
		CalculatedMaxPitch = StartingCameraRotation.Pitch + FMath::Abs(MaxCameraPitch);
	}
}


// Called every frame
void ABSecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddPitchInput(DeltaTime);
	AddYawInput(DeltaTime);
}


void ABSecurityCamera::AddPitchInput(float DeltaTime)
{
	if (Camera && FMath::Abs(PitchInput) > 0.f)
	{
		CurrentInterpPitchInput = FMath::FInterpTo(CurrentInterpPitchInput, PitchInput * PitchMoveSpeed, DeltaTime, PitchInterpSpeed);
		FRotator CombineRotation = Camera->GetComponentRotation() + FRotator(CurrentInterpPitchInput, 0.f, 0.f);
		FRotator SlerpRotation = FQuat::Slerp(FQuat(Camera->GetRelativeRotation()), FQuat(CombineRotation), 1.f).Rotator();

		if (SlerpRotation.Pitch < CalculatedMaxPitch && SlerpRotation.Pitch > CalculatedMinPitch)
		{
			Camera->SetWorldRotation(SlerpRotation);
		}
	}
	else
	{
		CurrentInterpPitchInput = 0.f;
	}
}


void ABSecurityCamera::AddYawInput(float DeltaTime)
{
	if (Camera && FMath::Abs(YawInput) > 0.f)
	{
		CurrentInterpYawInput = FMath::FInterpTo(CurrentInterpYawInput, YawInput * YawMoveSpeed, DeltaTime, YawInterpSpeed);
		FRotator CombineRotation = Camera->GetRelativeRotation() + FRotator(0.f, CurrentInterpYawInput, 0.f);
		FRotator SlerpRotation = FQuat::Slerp(FQuat(Camera->GetRelativeRotation()), FQuat(CombineRotation), 1.f).Rotator();

		if (SlerpRotation.Yaw < CalculatedMaxYaw && SlerpRotation.Yaw > CalculatedMinYaw)
		{
			Camera->SetRelativeRotation(SlerpRotation);
		}
	}
	else
	{
		CurrentInterpYawInput = 0.f;
	}
}




