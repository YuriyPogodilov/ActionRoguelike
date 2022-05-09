// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Dash")
	float ExplodeDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float TeleportDelay = 0.2f;

	FTimerHandle TimerHandle_Explode;

	virtual void BeginPlay() override;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();
};
