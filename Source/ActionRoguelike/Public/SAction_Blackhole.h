// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction_ProjectileAttack.h"
#include "SAction_Blackhole.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Blackhole : public USAction_ProjectileAttack
{
	GENERATED_BODY()
	
public:

	USAction_Blackhole();
	
	virtual bool CanStart_Implementation(AActor* Instigator) const override;

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	float RageCost;
};
