// Fill out your copyright notice in the Description page of Project Settings.


#include "BZombie.h"

#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"

#include "BlindTrustTypes.h"
#include "BBlindPlayerCharacter.h"
#include "BGameMode.h"

// Sets default values
ABZombie::ABZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CaptureSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CaptureSphere"));
	CaptureSphere->SetupAttachment(GetMesh());
	CaptureSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CaptureSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CaptureSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void ABZombie::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ENetRole::ROLE_Authority && CaptureSphere)
	{
		CaptureSphere->OnComponentBeginOverlap.AddDynamic(this, &ABZombie::OnCaptureSphereBeginOverlap);
	}
	
}

// Called every frame
void ABZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABZombie::OnCaptureSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABBlindPlayerCharacter* BlindPlayerCharacter = Cast<ABBlindPlayerCharacter>(OtherActor);
	if (BlindPlayerCharacter)
	{
		BlindPlayerCharacter->SetIsCaptured(true);
		ABGameMode* BGameMode = Cast<ABGameMode>(GetWorld()->GetAuthGameMode());
		if (BGameMode)
		{
			BGameMode->BlindPlayerCaught();
		}
	}


	//if (OtherActor && OtherActor->ActorHasTag(BLIND_PLAYER_TAG))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Caught Player"));
	//	APawn* p = Cast<APawn>(OtherActor);
	//	if (p)
	//	{
	//		APlayerController* PC =	Cast<APlayerController>(p->GetController());
	//		if (PC)
	//		{
	//			PC->SetInputMode(FInputModeUIOnly());
	//		}
	//	}

	//}
}

