// Fill out your copyright notice in the Description page of Project Settings.


#include "ZDefaultStartPlayer.h"

// Sets default values
AZDefaultStartPlayer::AZDefaultStartPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZDefaultStartPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZDefaultStartPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZDefaultStartPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

