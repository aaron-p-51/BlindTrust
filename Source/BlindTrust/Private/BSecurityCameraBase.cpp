// Fill out your copyright notice in the Description page of Project Settings.


#include "BSecurityCameraBase.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ABSecurityCameraBase::ABSecurityCameraBase()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MonitorScreen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonitorScreen"));
	MonitorScreen->SetupAttachment(GetRootComponent());

	CameraMount = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMount"));
	CameraMount->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camera"));
	Camera->SetupAttachment(CameraMount);
}

// Called when the game starts or when spawned
void ABSecurityCameraBase::BeginPlay()
{
	Super::BeginPlay();

	SetupDynamicMonitorMaterial();
}


void ABSecurityCameraBase::SetupDynamicMonitorMaterial()
{
	if (MonitorScreen)
	{
		MonitorScreenMaterialBase = MonitorScreen->GetMaterial(0);
		MonitorScreenDynInstance = UMaterialInstanceDynamic::Create(MonitorScreenMaterialBase, this);
		MonitorScreen->SetMaterial(0, MonitorScreenDynInstance);
	}
}


bool ABSecurityCameraBase::GetCameraLocationAndRotation(FVector& Location, FRotator& Rotation) const
{
	if (Camera)
	{
		Location = Camera->GetComponentLocation();
		Rotation = Camera->GetComponentRotation();
		return true;
	}

	return false;
}


FRotator ABSecurityCameraBase::GetCameraRotation() const
{
	if (Camera)
	{
		return Camera->GetComponentRotation();
	}

	return FRotator();
}

