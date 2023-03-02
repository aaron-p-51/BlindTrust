// Fill out your copyright notice in the Description page of Project Settings.


#include "ZSecurityCameraBase.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AZSecurityCameraBase::AZSecurityCameraBase()
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
void AZSecurityCameraBase::BeginPlay()
{
	Super::BeginPlay();

	SetupDynamicMonitorMaterial();
}


void AZSecurityCameraBase::SetupDynamicMonitorMaterial()
{
	if (MonitorScreen)
	{
		MonitorScreenMaterialBase = MonitorScreen->GetMaterial(0);
		MonitorScreenDynInstance = UMaterialInstanceDynamic::Create(MonitorScreenMaterialBase, this);
		MonitorScreen->SetMaterial(0, MonitorScreenDynInstance);
	}
}


bool AZSecurityCameraBase::GetCameraLocationAndRotation(FVector& Location, FRotator& Rotation) const
{
	if (Camera)
	{
		Location = Camera->GetComponentLocation();
		Rotation = Camera->GetComponentRotation();
		return true;
	}

	return false;
}


FRotator AZSecurityCameraBase::GetCameraRotation() const
{
	if (Camera)
	{
		return Camera->GetComponentRotation();
	}

	return FRotator();
}

