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

	MinCameraPitchOffset = -70.f;
	MaxCameraPitchOffset = 70.f;
	StartingPitchOffset = 0.f;
	PitchMoveSpeed = 1.f;
	PitchInterpSpeed = 0.4f;

	MinCameraYawOffset = -70.f;
	MaxCameraYawOffset = 70.f;
	StartingYawOffset = 0.f;
	YawMoveSpeed = 1.f;
	YawInterpSpeed = 0.4f;

	CurrentYawOffset = 0.f;
	CurrentPitchOffset = 0.f;
}


// Called when the game starts or when spawned
void ABSecurityCamera::BeginPlay()
{
	Super::BeginPlay();

	SetInitialValues();
}


void ABSecurityCamera::SetInitialValues()
{
	if (Camera)
	{
		if (StartingPitchOffset <= MaxCameraPitchOffset && StartingPitchOffset >= MinCameraPitchOffset)
		{
			CurrentPitchOffset = StartingPitchOffset;
		}

		if (StartingYawOffset <= MaxCameraYawOffset && StartingYawOffset >= MinCameraYawOffset)
		{
			CurrentYawOffset = StartingYawOffset;
		}

		Camera->AddRelativeRotation(FRotator(CurrentPitchOffset, CurrentYawOffset, 0.f));
	}
}


// Called every frame
void ABSecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyPitchInput(DeltaTime);
	ApplyYawInput(DeltaTime);
}




void ABSecurityCamera::ApplyPitchInput(float DeltaTime)
{
	if (Camera && PitchInput != 0.f)
	{
		if (PitchInput < 0.f && CurrentPitchOffset <= MinCameraPitchOffset) return;
		else if (PitchInput > 0.f && CurrentPitchOffset >= MaxCameraPitchOffset) return;

		CurrentInterpPitchInput = FMath::FInterpTo(CurrentInterpPitchInput, PitchInput * PitchMoveSpeed, DeltaTime, PitchInterpSpeed);
		Camera->AddRelativeRotation(FRotator(CurrentInterpPitchInput, 0.f, 0.f));
		CurrentPitchOffset += CurrentInterpPitchInput;
	}
	else
	{
		CurrentInterpPitchInput = 0.f;
	}
}


void ABSecurityCamera::ApplyYawInput(float DeltaTime)
{
	if (Camera && YawInput != 0.f)
	{
		if (YawInput < 0.f && CurrentYawOffset <= MinCameraYawOffset) return;
		else if (YawInput > 0.f && CurrentYawOffset >= MaxCameraYawOffset) return;

		CurrentInterpYawInput = FMath::FInterpTo(CurrentInterpYawInput, YawInput * YawMoveSpeed, DeltaTime, YawInterpSpeed);
		
		Camera->AddRelativeRotation(FRotator(0.f, CurrentInterpYawInput, 0.f));
		CurrentYawOffset += CurrentInterpYawInput;		
	}
	else
	{
		CurrentInterpYawInput = 0.f;
	}
}





