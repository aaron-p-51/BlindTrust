// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieChaseTypes.h"
#include "ZBlindPlayerPawn.generated.h"

UCLASS()
class WOLDTHECORRIDOR_API AZBlindPlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZBlindPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Components
	 */

	 UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	 class UCameraComponent* FirstPersonCamera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotateRootBone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator StartingAimRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimOffset_Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpAimOffset_Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETurningInPlace TurningInPlace;

	/**
	 * Player Input
	 * 
	 */
	UFUNCTION(BlueprintCallable)
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable)
	void MoveRight(float Value);



private:

	float CalculateSpeed() const;

	void TurnInPlace(float DeltaTime);

};
