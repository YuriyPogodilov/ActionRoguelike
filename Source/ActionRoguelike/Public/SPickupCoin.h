// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickableActorBase.h"
#include "SPickupCoin.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPickupCoin : public ASPickableActorBase
{
	GENERATED_BODY()

public:

	ASPickupCoin();

	virtual bool ApplyEffect_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	int32 AddingCreditsAmount;
};
