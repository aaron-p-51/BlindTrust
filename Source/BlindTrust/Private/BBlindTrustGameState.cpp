// Fill out your copyright notice in the Description page of Project Settings.


#include "BBlindTrustGameState.h"

#include "Net/UnrealNetwork.h"



void ABBlindTrustGameState::SetStartChaseTime()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		StartChaseTime = GetServerWorldTimeSeconds();
	}
}


void ABBlindTrustGameState::SetBlindPlayerCaughtTime()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		BlindPlayerCaughtTime = GetServerWorldTimeSeconds();
	}
}


void ABBlindTrustGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABBlindTrustGameState, StartChaseTime);
	DOREPLIFETIME(ABBlindTrustGameState, BlindPlayerCaughtTime);

}
