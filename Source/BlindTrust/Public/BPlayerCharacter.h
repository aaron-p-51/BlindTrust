// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlindTrustTypes.h"
#include "BPlayerCharacter.generated.h"

UCLASS()
class BLINDTRUST_API ABPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABPlayerCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Components
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	class UCameraComponent* CameraComp;

	/**
	 * Default Player Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<class UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Move;

	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Look;

	void Look(const FInputActionValue& Value);

	void AddMappingContext(TSoftObjectPtr<UInputMappingContext> Context, int32 Priority = 0);
	void RemoveMappingContext(TSoftObjectPtr<UInputMappingContext> Context);

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PossessedBy(AController* NewController) override;

	UFUNCTION(Client, Reliable)
	void ClientPossessed();
	

private:

	


};
