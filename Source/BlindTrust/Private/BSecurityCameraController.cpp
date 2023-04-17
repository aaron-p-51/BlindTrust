// Fill out your copyright notice in the Description page of Project Settings.


#include "BSecurityCameraController.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "BSecurityCamera.h"
#include "BGuidePlayerCharacter.h"

const FName SHOW_STATIC_PARAM = FName("ShowStatic");

// Sets default values
ABSecurityCameraController::ABSecurityCameraController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(GetRootComponent());
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	bIsSwitchingCameras = false;

}


// Called when the game starts or when spawned
void ABSecurityCameraController::BeginPlay()
{
	Super::BeginPlay();

	SetInitialCamera();

	if (InteractionVolume)
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &ABSecurityCameraController::OnInteractionVolumeBeginOverlap);
		InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &ABSecurityCameraController::OnInteractionVolumeEndOverlap);
	}
}


// Called every frame
void ABSecurityCameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Camera && CurrentSecurityCamera)
	{
		Camera->SetWorldRotation(CurrentSecurityCamera->GetCameraRotation());
	}
}


void ABSecurityCameraController::SetAllRenderTargetsActive(bool Value)
{
	for (auto SecurityCamera : SecurityCameras)
	{
		SecurityCamera->GetCamera()->bCaptureOnMovement = Value;
		SecurityCamera->GetCamera()->bCaptureEveryFrame = Value;
	}

	Camera->bCaptureOnMovement = Value;
	Camera->bCaptureEveryFrame = Value;
}

void ABSecurityCameraController::SetInitialCamera()
{
	if (SecurityCameras.Num() > 0 && Camera)
	{
		CurrentSecurityCamera = SecurityCameras[0];
		MoveCameraToCurrentSecurityCamera();
	}
}


void ABSecurityCameraController::MoveCameraToCurrentSecurityCamera()
{
	FVector CameraLocation;
	FRotator CameraRotation;
	if (CurrentSecurityCamera && CurrentSecurityCamera->GetCameraLocationAndRotation(CameraLocation, CameraRotation))
	{
		Camera->SetWorldLocationAndRotation(CameraLocation, CameraRotation);
	}
}


void ABSecurityCameraController::ShowStaticMonitorScreen(bool Value)
{
	if (MonitorScreenDynInstance)
	{
		float ShowStaticValue = Value ? 1.f : 0.f;
		MonitorScreenDynInstance->SetScalarParameterValue(SHOW_STATIC_PARAM, ShowStaticValue);
	}
}


void ABSecurityCameraController::SwitchCameraDelayComplete()
{
	MoveCameraToCurrentSecurityCamera();
	ShowStaticMonitorScreen(false);
	bIsSwitchingCameras = false;
}


void ABSecurityCameraController::OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto GuidePlayerCharacter = Cast<ABGuidePlayerCharacter>(OtherActor))
	{
		GuidePlayerCharacter->SetSecurityCameraController(this);
	}
}


void ABSecurityCameraController::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (auto GuidePlayerCharacter = Cast<ABGuidePlayerCharacter>(OtherActor))
	{
		GuidePlayerCharacter->SetSecurityCameraController(nullptr);
	}
}


////////////////////////////////////////////////////
//
// Input
//
////////////////////////////////////////////////////
void ABSecurityCameraController::SwitchNextCamera()
{
	if (!bIsSwitchingCameras && SecurityCameras.Num() > 1)
	{
		bIsSwitchingCameras = true;
		
		const int32 CurrentSecurityCameraIndex = SecurityCameras.IndexOfByKey(CurrentSecurityCamera);
		const int32 NextSecurityCameraIndex = (CurrentSecurityCameraIndex + 1) % SecurityCameras.Num();

		CurrentSecurityCamera = SecurityCameras[NextSecurityCameraIndex];

		if (CameraSwitchDelay > 0.f)
		{
			ShowStaticMonitorScreen(true);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_SwitchCameraDelay, this, &ABSecurityCameraController::SwitchCameraDelayComplete, CameraSwitchDelay, false);
		}
		else
		{
			MoveCameraToCurrentSecurityCamera();
		}
	}
}


void ABSecurityCameraController::SwitchPreviousCamera()
{
	if (!bIsSwitchingCameras && SecurityCameras.Num() > 1)
	{
		bIsSwitchingCameras = true;

		const int32 CurrentSecurityCameraIndex = SecurityCameras.IndexOfByKey(CurrentSecurityCamera);
		const int32 NextSecurityCameraIndex = (CurrentSecurityCameraIndex - 1 + SecurityCameras.Num()) % SecurityCameras.Num();

		CurrentSecurityCamera = SecurityCameras[NextSecurityCameraIndex];

		if (CameraSwitchDelay > 0.f)
		{
			ShowStaticMonitorScreen(true);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_SwitchCameraDelay, this, &ABSecurityCameraController::SwitchCameraDelayComplete, CameraSwitchDelay, false);
		}
		else
		{
			MoveCameraToCurrentSecurityCamera();
		}
	}
}


void ABSecurityCameraController::AddCurrentCameraPitch(float Value)
{
	if (!bIsSwitchingCameras && CurrentSecurityCamera)
	{
		CurrentSecurityCamera->AddPitch(Value);
	}
}


void ABSecurityCameraController::AddCurrentCameraYaw(float Value)
{
	if (!bIsSwitchingCameras && CurrentSecurityCamera)
	{
		CurrentSecurityCamera->AddYaw(Value);
	}
}
