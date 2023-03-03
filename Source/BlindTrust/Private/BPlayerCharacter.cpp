// Fill out your copyright notice in the Description page of Project Settings.


#include "BPlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"

#include "ZSecurityCameraController.h"

// Sets default values
ABPlayerCharacter::ABPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ABPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetController());
	if (LocalPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalPlayer: %s"), *LocalPlayer->GetName());
	}

	if (IsLocallyControlled())
	{
		AddMappingContext(DefaultInputMapping);
	}

}


// Called every frame
void ABPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}


// Called to bind functionality to input
void ABPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABPlayerCharacter::Move);
		Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABPlayerCharacter::Look);
		Input->BindAction(IA_SwitchCamera, ETriggerEvent::Triggered, this, &ABPlayerCharacter::SwitchCamera);
		Input->BindAction(IA_RotateCamera, ETriggerEvent::Triggered, this, &ABPlayerCharacter::RotateCamera);
		Input->BindAction(IA_RotateCamera, ETriggerEvent::Completed, this, &ABPlayerCharacter::RotateCamera);
	}

}


void ABPlayerCharacter::AddMappingContext(TSoftObjectPtr<UInputMappingContext> Context, int32 Priority)
{
	if (Context.IsNull()) return;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(Context.LoadSynchronous(), Priority);
		}
	}
}


void ABPlayerCharacter::RemoveMappingContext(TSoftObjectPtr<UInputMappingContext> Context)
{
	if (Context.IsNull()) return;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(Context.Get());
		}
	}
}


void ABPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D CurrentValue = Value.Get<FVector2D>();
	if (!CurrentValue.IsNearlyZero())
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

		const FVector DirectionForward(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(DirectionForward, CurrentValue.Y);

		const FVector DirectionRight(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(DirectionRight, CurrentValue.X);
	}
}


void ABPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D CurrentValue = Value.Get<FVector2D>();
	if (CurrentValue.X != 0)
	{
		AddControllerYawInput(CurrentValue.X);
	}

	if (CurrentValue.Y != 0)
	{
		AddControllerPitchInput(-CurrentValue.Y);
	}
}


void ABPlayerCharacter::SwitchCamera(const FInputActionValue& Value)
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


void ABPlayerCharacter::RotateCamera(const FInputActionValue& Value)
{
	if (SecurityCameraController)
	{
		const FVector2D CurrentValue = Value.Get<FVector2D>();
		SecurityCameraController->AddCurrentCameraPitch(CurrentValue.Y);
		SecurityCameraController->AddCurrentCameraYaw(CurrentValue.X);
	}
}


void ABPlayerCharacter::AddSecurityCameraControlMappingInput()
{
	AddMappingContext(SecurityCameraControlMapping, 0);
}


void ABPlayerCharacter::RemoveSecurityCameraControlMappingInput()
{
	RemoveMappingContext(SecurityCameraControlMapping);
}


void ABPlayerCharacter::SetSecurityCameraController(AZSecurityCameraController* Value)
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



