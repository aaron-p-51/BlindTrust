// Fill out your copyright notice in the Description page of Project Settings.


#include "BBlindPlayerCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

#include "BlindTrustTypes.h"

ABBlindPlayerCharacter::ABBlindPlayerCharacter()
{
	CameraComp->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 304.f), FRotator(0.f, 90.f, 0.f).Quaternion());
	Tags.Add(BLIND_PLAYER_TAG);
	IsCaptured = false;
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

void ABBlindPlayerCharacter::SetIsCaptured(bool Value)
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		IsCaptured = Value;

		// For listen server
		OnRep_IsCaptured();
	}
}


void ABBlindPlayerCharacter::OnRep_IsCaptured()
{
	if (IsCaptured)
	{
		RemoveMappingContext(DefaultInputMapping);
	}
}

void ABBlindPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABBlindPlayerCharacter, IsCaptured);
}


