// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ZPlayerCharacter.generated.h"

UCLASS()
class WOLDTHECORRIDOR_API AZPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Components
	 */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* FirstPersonCamera;


	/**
	 * Player Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<class UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Move;

	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Look;

	void Look(const FInputActionValue& Value);

	



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

protected:

	void AddMappingContext(TSoftObjectPtr<UInputMappingContext> Context, int32 Priority = 0);
	void RemoveMappingContext(TSoftObjectPtr<UInputMappingContext> Context);


};
