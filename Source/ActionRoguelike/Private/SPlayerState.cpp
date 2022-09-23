// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"



ASPlayerState::ASPlayerState()
{
	Credits = 0;
}


int32 ASPlayerState::GetCredits() const
{
	return Credits;
}


bool ASPlayerState::ApplyCreditChange(AActor* InstigatorActor, int32 Delta)
{
	int32 OldCredits = Credits;
	int32 NewCredits = OldCredits + Delta;

	if (NewCredits < 0)
	{
		return false;
	}

	Credits = NewCredits;

	if (OnCreditChanged.IsBound())
	{
		OnCreditChanged.Broadcast(InstigatorActor, this, Credits, Delta);
	}

	return true;
}
