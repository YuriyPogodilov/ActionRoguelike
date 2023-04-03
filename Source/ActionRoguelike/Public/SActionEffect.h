// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:

	USActionEffect();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemaining() const;

	virtual void AddStack();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	bool bIsStackable{false};

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
	int32 NumberOfStacks{0};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	int32 MaximumStacks{1};

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodEffect(AActor* Instigator);
};
