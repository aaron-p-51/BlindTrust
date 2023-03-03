// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlindTrustTypes.h"
#include "InputActionValue.h"
#include "BBlindPlayerPawn.generated.h"

UCLASS()
class BLINDTRUST_API ABBlindPlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABBlindPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Components
	 */

	 UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	 class UCameraComponent* FirstPersonCamera;

	 // Expose a mapping context as a property in your header file...
	 UPROPERTY(EditAnywhere, Category = "Input")
	 TSoftObjectPtr<class UInputMappingContext> InputMapping;

	 UPROPERTY(EditAnywhere, Category = "Input")
	 class UInputAction* IA_MoveAction;

	 


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

	

	void Move(const FInputActionValue& Value);

private:

	float CalculateSpeed() const;

	void TurnInPlace(float DeltaTime);

};
