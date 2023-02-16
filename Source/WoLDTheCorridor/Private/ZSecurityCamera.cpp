// Fill out your copyright notice in the Description page of Project Settings.


#include "ZSecurityCamera.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
AZSecurityCamera::AZSecurityCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MonitorScreen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonitorScreen"));
	MonitorScreen->SetupAttachment(GetRootComponent());

	CameraMount = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMount"));
	CameraMount->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camera"));
	Camera->SetupAttachment(CameraMount);

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
void AZSecurityCamera::BeginPlay()
{
	Super::BeginPlay();

	ValidateInstanceValues();
}


void AZSecurityCamera::ValidateInstanceValues()
{
	// Validate Pitch values set for instances of this class
	if (MinCameraPitch < -70.f || MinCameraPitch > 70.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has invalid MinCameraPitch: %f, will set to -70.0"), *GetName(), MinCameraPitch);
		MinCameraPitch = -89.f;
	}
	if (MaxCameraPitch < -70.f || MaxCameraPitch > 70.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has invalid MaxCameraPitch: %f, will set to 70.0"), *GetName(), MaxCameraPitch);
		MaxCameraPitch = -89.f;
	}
	if (MinCameraPitch > MaxCameraPitch)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s MinCameraPitch is greater than MaxCameraPitch"), *GetName());
	}

	// Validate yaw values set for instances of this class
	if (MinCameraYaw < -89.f || MinCameraYaw > 89.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has invalid MinCameraYaw: %f, will set -70.0"), *GetName(), MinCameraYaw);
		MinCameraYaw = -89.f;
	}
	if (MaxCameraYaw < -89.f || MaxCameraYaw > 89.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Has invalid MaxCameraYaw: %f, will set 70.0"), *GetName(), MaxCameraYaw);
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
void AZSecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddPitchInput(DeltaTime);
	AddYawInput(DeltaTime);
}


void AZSecurityCamera::AddPitchInput(float DeltaTime)
{
	if (Camera && FMath::Abs(PitchInput) > 0.f)
	{
		CurrentInterpPitchInput = FMath::FInterpTo(CurrentInterpPitchInput, PitchInput * PitchMoveSpeed, DeltaTime, PitchInterpSpeed);
		FRotator CombineRotation = Camera->GetComponentRotation() + FRotator(CurrentInterpPitchInput, 0.f, 0.f);

		if (CombineRotation.Pitch < CalculatedMaxPitch && CombineRotation.Pitch > CalculatedMinPitch)
		{
			Camera->SetWorldRotation(CombineRotation);			
		}
	}
	else
	{
		CurrentInterpPitchInput = 0.f;
	}
}


void AZSecurityCamera::AddYawInput(float DeltaTime)
{
	if (Camera && FMath::Abs(YawInput))
	{
		CurrentInterpYawInput = FMath::FInterpTo(CurrentInterpYawInput, YawInput * YawMoveSpeed, DeltaTime, YawInterpSpeed);
		FRotator CombineRotation = Camera->GetComponentRotation() + FRotator(0.f, CurrentInterpYawInput, 0.f);

		if (CombineRotation.Yaw < CalculatedMaxYaw && CombineRotation.Yaw > CalculatedMinYaw)
		{
			Camera->SetWorldRotation(CombineRotation);
		}
	}
	else
	{
		CurrentInterpYawInput = 0.f;
	}
}


bool AZSecurityCamera::GetCameraLocationAndRotation(FVector& Location, FRotator& Rotation) const
{
	if (Camera)
	{
		Location = Camera->GetComponentLocation();
		Rotation = Camera->GetComponentRotation();
		return true;
	}

	return false;
}


FRotator AZSecurityCamera::GetCameraRotation() const
{
	if (Camera)
	{
		return Camera->GetComponentRotation();
	}

	return FRotator();
}

