// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"

// Sets default values
AZPlayerCharacter::AZPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AZPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetController());
	if (LocalPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalPlayer: %s"), *LocalPlayer->GetName());
	}

}


// Called every frame
void AZPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		AddMappingContext(DefaultInputMapping);
	}

}


// Called to bind functionality to input
void AZPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AZPlayerCharacter::Move);
		Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AZPlayerCharacter::Look);
	}

}


void AZPlayerCharacter::AddMappingContext(TSoftObjectPtr<UInputMappingContext> Context, int32 Priority)
{
	if (Context.IsNull()) return;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(Context.Get(), Priority);
		}
	}
}


void AZPlayerCharacter::RemoveMappingContext(TSoftObjectPtr<UInputMappingContext> Context)
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


void AZPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D CurrentValue = Value.Get<FVector2D>();
	if (!CurrentValue.IsNearlyZero())
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

		const FVector DirectionForward(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(DirectionForward, CurrentValue.Y);

		const FVector DirectionRight(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(DirectionForward, CurrentValue.X);
	}


}


void AZPlayerCharacter::Look(const FInputActionValue& Value)
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

