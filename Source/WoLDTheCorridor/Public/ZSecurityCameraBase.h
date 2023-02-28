// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSecurityCameraBase.generated.h"

UCLASS()
class WOLDTHECORRIDOR_API AZSecurityCameraBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZSecurityCameraBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Components
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* MonitorScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneCaptureComponent2D* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* CameraMount;

	/**
	 * Monitor VFX
	 */
	UPROPERTY()
	class UMaterialInterface* MonitorScreenMaterialBase;

	UPROPERTY()
	class UMaterialInstanceDynamic* MonitorScreenDynInstance;


public:	


	/** Get location and rotation of camera component */
	bool GetCameraLocationAndRotation(FVector& Location, FRotator& Rotation) const;

	/** Get rotation of camera component */
	FRotator GetCameraRotation() const;

private:

	
	void SetupDynamicMonitorMaterial();


};
