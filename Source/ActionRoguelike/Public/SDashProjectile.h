// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "SDashProjectile.generated.h"

class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Dash")
	float ExplodeDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float TeleportDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	UParticleSystem* ExplodeEffect;

	FTimerHandle TimerHandle_Explode;
	FTimerHandle TimerHandle_Teleport;

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void Explode();

	void Teleport();

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult);
};
