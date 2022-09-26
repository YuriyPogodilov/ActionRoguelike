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

void ASPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
	{
		return;
	}

	Credits += Delta;

	if (OnCreditChanged.IsBound())
	{
		OnCreditChanged.Broadcast(this, Credits, Delta);
	}
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
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

	if (OnCreditChanged.IsBound())
	{
		OnCreditChanged.Broadcast(this, Credits, -Delta);
	}

	return true;
}
