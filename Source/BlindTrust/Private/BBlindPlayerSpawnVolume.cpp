// Fill out your copyright notice in the Description page of Project Settings.


#include "BBlindPlayerSpawnVolume.h"

#include "Components/BoxComponent.h"
#include "GameFramework/PlayerStart.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABBlindPlayerSpawnVolume::ABBlindPlayerSpawnVolume()
{

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SetRootComponent(SpawnVolume);
	SpawnVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}


// Called when the game starts or when spawned
void ABBlindPlayerSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	//FTransform T;
	//GetRandomSpawnPoint(T);
}





bool ABBlindPlayerSpawnVolume::GetRandomSpawnPoint(FTransform& SpawnTransform) const
{
	if (SpawnVolume)
	{
		FVector SpawnVolumeLocation = SpawnVolume->GetComponentLocation();
		
		float SpawnX = FMath::RandRange(SpawnVolume->GetScaledBoxExtent().X, -SpawnVolume->GetScaledBoxExtent().X) + SpawnVolumeLocation.X;
		float SpawnY = FMath::RandRange(SpawnVolume->GetScaledBoxExtent().Y, -SpawnVolume->GetScaledBoxExtent().Y) + SpawnVolumeLocation.Y;

		FVector LineTraceStart = FVector(SpawnX, SpawnY, SpawnVolumeLocation.Z + SpawnVolume->GetScaledBoxExtent().Z);
		FVector LineTraceEnd = FVector(LineTraceStart.X, LineTraceStart.Y, LineTraceStart.Z - (SpawnVolume->GetScaledBoxExtent().Z * 2.f));

		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Visibility))
		{
			if (HitResult.bBlockingHit)
			{
				DrawDebugSphere(GetWorld(), HitResult.Location, 2.f, 12, FColor::Yellow, true);


				SpawnTransform = FTransform(GetRandomSpawnRotation(), FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z + SpawnZAxisOffset));
				return true;
			}
		}
	}

	return false;
}


FQuat ABBlindPlayerSpawnVolume::GetRandomSpawnRotation() const
{
	return FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f).Quaternion();
}



