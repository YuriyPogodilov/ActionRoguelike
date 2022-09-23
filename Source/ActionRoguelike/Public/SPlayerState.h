// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreditChanged, AActor*, InstigatorActor, APlayerState*, Owner, int32, NewCredit, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ASPlayerState();

	UFUNCTION(BlueprintCallable)
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable)
	bool ApplyCreditChange(AActor* InstigatorActor, int32 Delta);

	UPROPERTY(BlueprintAssignable)
	FOnCreditChanged OnCreditChanged;

protected:

	UPROPERTY(VisibleAnywhere)
	int32 Credits;
};
