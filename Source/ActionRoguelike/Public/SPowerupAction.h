// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickableActorBase.h"
#include "SPowerupAction.generated.h"

class USAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerupAction : public ASPickableActorBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<USAction> ActionToPickup;

public:
	
	virtual bool ApplyEffect_Implementation(APawn* InstigatorPawn) override;
};
