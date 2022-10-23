// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"


ASPlayerState::ASPlayerState()
{
	Credits = 0;

	bReplicates = true;
}


int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::AddCredits(int32 Delta)
{
	if (!HasAuthority())
	{
		return;
	}
	
	if (!ensure(Delta > 0.0f))
	{
		return;
	}

	Credits += Delta;

	OnCreditChanged.Broadcast(this, Credits, Delta);
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if (!HasAuthority())
	{
		return false;
	}

	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (Delta > Credits)
	{
		// not enough credits
		return false;
	}

	Credits -= Delta;

	OnCreditChanged.Broadcast(this, Credits, -Delta);

	return true;
}


void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (ensure(SaveObject))
	{
		SaveObject->Credits = Credits;
	}
}


void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (ensure(SaveObject))
	{
		AddCredits(SaveObject->Credits);
	}
}


void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditChanged.Broadcast(this, Credits, Credits - OldCredits);
}

// void ASPlayerState::MulticastCreditChanged_Implementation(int32 Delta)
// {
// 	OnCreditChanged.Broadcast(this, Credits, Delta);
// }

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}
