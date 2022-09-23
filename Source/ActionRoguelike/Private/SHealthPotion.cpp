// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASHealthPotion::ASHealthPotion()
{
	HealthRestoreAmount = 30.0f;
	Cost = 30.0f;
}

bool ASHealthPotion::ApplyEffect_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return false;
	}

	ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (!PlayerState)
	{
		return false;
	}

	if (!PlayerState->ApplyCreditChange(this, -Cost))
	{
		return false;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		return AttributeComp->ApplyHealthChange(this, HealthRestoreAmount);
	}

	return false;
}
