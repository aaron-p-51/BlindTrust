// Fill out your copyright notice in the Description page of Project Settings.


#include "BBlindPlayerCharacter.h"

#include "Blueprint/UserWidget.h"

void ABBlindPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	auto PC = Cast<APlayerController>(GetController());



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
