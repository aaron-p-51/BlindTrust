// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPlayerCharacter.h"
#include "BGuidePlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BLINDTRUST_API ABGuidePlayerCharacter : public ABPlayerCharacter
{
	GENERATED_BODY()

public:

	ABGuidePlayerCharacter();

protected:

	/**
	 * Security camera control input
	 */

	 UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<class UInputMappingContext> SecurityCameraControlMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_SwitchCamera;

	void SwitchCamera(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_RotateCamera;

	void RotateCamera(const FInputActionValue& Value);


	UPROPERTY(VisibleAnywhere)
	class ABSecurityCameraController* SecurityCameraController;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddSecurityCameraControlMappingInput();
	void RemoveSecurityCameraControlMappingInput();

	void SetSecurityCameraController(ABSecurityCameraController* Value);


	
};
