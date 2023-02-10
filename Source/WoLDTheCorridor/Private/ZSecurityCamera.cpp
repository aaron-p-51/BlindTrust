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

	Camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AZSecurityCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZSecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool AZSecurityCamera::GetCameraLocationAndRotation(FVector& Location, FRotator& Rotation)
{
	if (Camera)
	{
		Location = Camera->GetComponentLocation();
		Rotation = Camera->GetComponentRotation();
		return true;
	}

	return false;
}

