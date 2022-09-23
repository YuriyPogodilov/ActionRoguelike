// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupCoin.h"

#include "SPlayerState.h"

ASPickupCoin::ASPickupCoin()
{
	AddingCreditsAmount = 30;
}

bool ASPickupCoin::ApplyEffect_Implementation(APawn* InstigatorPawn)
{
	ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (!PlayerState)
	{
		return false;
	}

	return PlayerState->ApplyCreditChange(this, AddingCreditsAmount);
}
