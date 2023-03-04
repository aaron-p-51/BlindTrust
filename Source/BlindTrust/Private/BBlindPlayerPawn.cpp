// Fill out your copyright notice in the Description page of Project Settings.


#include "BBlindPlayerPawn.h"

// Engine Includes
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/Subsystem.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"


// Game Includes



// Sets default values
ABBlindPlayerPawn::ABBlindPlayerPawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());


	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

// Called when the game starts or when spawned
void ABBlindPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping.Get(), 0);
		}
	}
	

	
	
}

// Called every frame
void ABBlindPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	float Speed = CalculateSpeed();
	if (Speed == 0.f)
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AimOffset_Yaw = DeltaAimRotation.Yaw;

		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAimOffset_Yaw = AimOffset_Yaw;
		}

		//bUseControllerRotationYaw = false;

		TurnInPlace(DeltaTime);
	}

	if (Speed > 0.f)
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AimOffset_Yaw = 0.f;
		//bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		
	}

}


// Called to bind functionality to input
void ABBlindPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_MoveAction, ETriggerEvent::Triggered, this, &ABBlindPlayerPawn::Move);
	}

}


void ABBlindPlayerPawn::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));

		AddMovementInput(Direction, Value);
	}
}


void ABBlindPlayerPawn::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));

		AddMovementInput(Direction, Value);
	}
}


void ABBlindPlayerPawn::Move(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();
	if (CurrentValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Move triggered"));
	}
}

float ABBlindPlayerPawn::CalculateSpeed() const
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;

	return Velocity.Size();
}


void ABBlindPlayerPawn::TurnInPlace(float DeltaTime)
{
	if (AimOffset_Yaw > 40.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AimOffset_Yaw < -40.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}

	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAimOffset_Yaw = FMath::FInterpTo(InterpAimOffset_Yaw, 0.f, DeltaTime, 4.f);
		AimOffset_Yaw = InterpAimOffset_Yaw;
		if (FMath::Abs(AimOffset_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

