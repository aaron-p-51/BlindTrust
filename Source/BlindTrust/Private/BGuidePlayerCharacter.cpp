// Fill out your copyright notice in the Description page of Project Settings.


#include "BGuidePlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"

#include "BSecurityCameraController.h"

void ABGuidePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_SwitchCamera, ETriggerEvent::Triggered, this, &ABGuidePlayerCharacter::SwitchCamera);
		Input->BindAction(IA_RotateCamera, ETriggerEvent::Triggered, this, &ABGuidePlayerCharacter::RotateCamera);
		Input->BindAction(IA_RotateCamera, ETriggerEvent::Completed, this, &ABGuidePlayerCharacter::RotateCamera);
	}

}


void ABGuidePlayerCharacter::SwitchCamera(const FInputActionValue& Value)
{
	if (SecurityCameraController)
	{
		const float CurrentValue = Value.Get<float>();
		UE_LOG(LogTemp, Warning, TEXT("SwitchCamera: %f"), CurrentValue);
		if (CurrentValue > 0.f)
		{
			SecurityCameraController->SwitchNextCamera();
		}
		else if (CurrentValue < 0.f)
		{
			SecurityCameraController->SwitchPreviousCamera();
		}
	}
}


void ABGuidePlayerCharacter::RotateCamera(const FInputActionValue& Value)
{
	if (SecurityCameraController)
	{
		const FVector2D CurrentValue = Value.Get<FVector2D>();
		SecurityCameraController->AddCurrentCameraPitch(CurrentValue.Y);
		SecurityCameraController->AddCurrentCameraYaw(CurrentValue.X);
	}
}


void ABGuidePlayerCharacter::AddSecurityCameraControlMappingInput()
{
	AddMappingContext(SecurityCameraControlMapping, 0);
}


void ABGuidePlayerCharacter::RemoveSecurityCameraControlMappingInput()
{
	RemoveMappingContext(SecurityCameraControlMapping);
}

void ABGuidePlayerCharacter::SetSecurityCameraController(ABSecurityCameraController* Value)
{
	if (Value)
	{
		SecurityCameraController = Value;
		AddMappingContext(SecurityCameraControlMapping, 0);
		UE_LOG(LogTemp, Warning, TEXT("SetSecurityCameraController"));
	}
	else
	{
		SecurityCameraController = nullptr;
		RemoveMappingContext(SecurityCameraControlMapping);
		UE_LOG(LogTemp, Warning, TEXT("RemoveSecurityCameraController"));
	}
}