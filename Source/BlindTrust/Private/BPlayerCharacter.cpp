// Fill out your copyright notice in the Description page of Project Settings.


#include "BPlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"


// Sets default values
ABPlayerCharacter::ABPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(GetMesh());
	CameraComp->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));
	CameraComp->bUsePawnControlRotation = true;


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
	}

}


void ABPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ClientPossessed();
}


void ABPlayerCharacter::ClientPossessed_Implementation()
{
	if (IsLocallyControlled())
	{
		AddMappingContext(DefaultInputMapping);
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


















