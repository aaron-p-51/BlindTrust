// Fill out your copyright notice in the Description page of Project Settings.


#include "ZSecurityCameraController.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "ZSecurityCamera.h"
#include "ZPlayerCharacter.h"

const FName SHOW_STATIC_PARAM = FName("ShowStatic");

// Sets default values
AZSecurityCameraController::AZSecurityCameraController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(GetRootComponent());
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	MonitorScreen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonitorScreen"));
	MonitorScreen->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());

	bIsSwitchingCameras = false;
}


// Called when the game starts or when spawned
void AZSecurityCameraController::BeginPlay()
{
	Super::BeginPlay();

	//MonitorScreenDynInstance = UMaterialInstanceDynamic::Create(MonitorScreen->GetMaterial(0), MonitorScreen);
	
	MonitorScreenMaterialBase = MonitorScreen->GetMaterial(0);
	MonitorScreenDynInstance = UMaterialInstanceDynamic::Create(MonitorScreenMaterialBase, this);
	MonitorScreen->SetMaterial(0, MonitorScreenDynInstance);

	SetInitialCamera();

	if (InteractionVolume)
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AZSecurityCameraController::OnInteractionVolumeBeginOverlap);
		InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AZSecurityCameraController::OnInteractionVolumeEndOverlap);
	}
}


// Called every frame
void AZSecurityCameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Camera && CurrentSecurityCamera)
	{
		Camera->SetWorldRotation(CurrentSecurityCamera->GetCameraRotation());
	}
}


void AZSecurityCameraController::SetInitialCamera()
{
	if (SecurityCameras.Num() > 0 && Camera)
	{
		CurrentSecurityCamera = SecurityCameras[0];
		MoveCameraToCurrentSecurityCamera();
	}
}


void AZSecurityCameraController::MoveCameraToCurrentSecurityCamera()
{
	FVector CameraLocation;
	FRotator CameraRotation;
	if (CurrentSecurityCamera && CurrentSecurityCamera->GetCameraLocationAndRotation(CameraLocation, CameraRotation))
	{
		Camera->SetWorldLocationAndRotation(CameraLocation, CameraRotation);
	}
}


void AZSecurityCameraController::ShowStaticMonitorScreen(bool Value)
{
	if (MonitorScreenDynInstance)
	{
		float ShowStaticValue = Value ? 1.f : 0.f;
		MonitorScreenDynInstance->SetScalarParameterValue(SHOW_STATIC_PARAM, ShowStaticValue);
	}
}


void AZSecurityCameraController::SwitchCameraDelayComplete()
{
	MoveCameraToCurrentSecurityCamera();
	ShowStaticMonitorScreen(false);
	bIsSwitchingCameras = false;
}


void AZSecurityCameraController::OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AZPlayerCharacter* ZPlayerCharacter = Cast<AZPlayerCharacter>(OtherActor))
	{
		ZPlayerCharacter->SetSecurityCameraController(this);
	}
}


void AZSecurityCameraController::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AZPlayerCharacter* ZPlayerCharacter = Cast<AZPlayerCharacter>(OtherActor))
	{
		ZPlayerCharacter->SetSecurityCameraController(nullptr);
	}
}


////////////////////////////////////////////////////
//
// Input
//
////////////////////////////////////////////////////
void AZSecurityCameraController::SwitchNextCamera()
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
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_SwitchCameraDelay, this, &AZSecurityCameraController::SwitchCameraDelayComplete, CameraSwitchDelay, false);
		}
		else
		{
			MoveCameraToCurrentSecurityCamera();
		}
	}
}


void AZSecurityCameraController::SwitchPreviousCamera()
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
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_SwitchCameraDelay, this, &AZSecurityCameraController::SwitchCameraDelayComplete, CameraSwitchDelay, false);
		}
		else
		{
			MoveCameraToCurrentSecurityCamera();
		}
	}
}


void AZSecurityCameraController::AddCurrentCameraPitch(float Value)
{
	if (!bIsSwitchingCameras && CurrentSecurityCamera)
	{
		CurrentSecurityCamera->AddPitch(Value);
	}
}


void AZSecurityCameraController::AddCurrentCameraYaw(float Value)
{
	if (!bIsSwitchingCameras && CurrentSecurityCamera)
	{
		CurrentSecurityCamera->AddYaw(Value);
	}
}
