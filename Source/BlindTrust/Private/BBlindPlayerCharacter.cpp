// Fill out your copyright notice in the Description page of Project Settings.


#include "BBlindPlayerCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"

ABBlindPlayerCharacter::ABBlindPlayerCharacter()
{
	CameraComp->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 304.f), FRotator(0.f, 90.f, 0.f).Quaternion());
}

void ABBlindPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		if (BlindOverlayWidgetClass)
		{
			auto BlindOverlay = CreateWidget<UUserWidget>(GetWorld(), BlindOverlayWidgetClass);
			if (BlindOverlay)
			{
				BlindOverlay->AddToViewport(0);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Overlay Widget"));
			}
		}

		GetMesh()->SetVisibility(false);
	}
}
