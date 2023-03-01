// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGuidePlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"

#include "ZSecurityCameraController.h"

void AZGuidePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_SwitchCamera, ETriggerEvent::Triggered, this, &AZGuidePlayerCharacter::SwitchCamera);
		Input->BindAction(IA_RotateCamera, ETriggerEvent::Triggered, this, &AZGuidePlayerCharacter::RotateCamera);
		Input->BindAction(IA_RotateCamera, ETriggerEvent::Completed, this, &AZGuidePlayerCharacter::RotateCamera);
	}

}


void AZGuidePlayerCharacter::SwitchCamera(const FInputActionValue& Value)
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


void AZGuidePlayerCharacter::RotateCamera(const FInputActionValue& Value)
{
	if (SecurityCameraController)
	{
		const FVector2D CurrentValue = Value.Get<FVector2D>();
		SecurityCameraController->AddCurrentCameraPitch(CurrentValue.Y);
		SecurityCameraController->AddCurrentCameraYaw(CurrentValue.X);
	}
}


void AZGuidePlayerCharacter::AddSecurityCameraControlMappingInput()
{
	AddMappingContext(SecurityCameraControlMapping, 0);
}


void AZGuidePlayerCharacter::RemoveSecurityCameraControlMappingInput()
{
	RemoveMappingContext(SecurityCameraControlMapping);
}


void AZGuidePlayerCharacter::SetSecurityCameraController(AZSecurityCameraController* Value)
{
	if (Value)
	{
		SecurityCameraController = Value;
		AddMappingContext(SecurityCameraControlMapping, 0);
	}
	else
	{
		SecurityCameraController = nullptr;
		RemoveMappingContext(SecurityCameraControlMapping);
	}
}