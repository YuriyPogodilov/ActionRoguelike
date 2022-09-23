// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickableActorBase.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickableActorBase
{
	GENERATED_BODY()

public:

	ASHealthPotion();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Potion")
	float HealthRestoreAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Potion")
	int32 Cost;

	virtual bool ApplyEffect_Implementation(APawn* InstigatorPawn) override;
};
