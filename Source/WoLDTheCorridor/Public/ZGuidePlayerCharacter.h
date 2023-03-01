// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZPlayerCharacter.h"
#include "ZGuidePlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class WOLDTHECORRIDOR_API AZGuidePlayerCharacter : public AZPlayerCharacter
{
	GENERATED_BODY()

protected:


	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<class UInputMappingContext> SecurityCameraControlMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_SwitchCamera;

	void SwitchCamera(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_RotateCamera;

	void RotateCamera(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere)
	class AZSecurityCameraController* SecurityCameraController;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void AddSecurityCameraControlMappingInput();
	void RemoveSecurityCameraControlMappingInput();

	void SetSecurityCameraController(AZSecurityCameraController* Value);
	
};
