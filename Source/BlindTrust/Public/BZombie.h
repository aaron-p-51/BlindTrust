// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BZombie.generated.h"

class USphereComponent;

UCLASS()
class BLINDTRUST_API ABZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABZombie();

protected:

	/**
	 * Components
	 */
	UPROPERTY(EditDefaultsOnly, Category="Components")
	USphereComponent* CaptureSphere;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UFUNCTION()
	void OnCaptureSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
