// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickableActorBase.generated.h"

class USphereComponent;


UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASPickableActorBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:
	ASPickableActorBase();

	void SetActive(bool bNewIsActive);

	void ShowUp();

protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ApplyEffect(APawn* InstigatorPawn);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshBase;

	FTimerHandle CooldownTimer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsActive = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CooldownTime = 10.0f;
};
