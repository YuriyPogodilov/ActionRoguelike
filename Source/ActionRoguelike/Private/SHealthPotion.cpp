// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASHealthPotion::ASHealthPotion()
{
	HealthRestoreAmount = 30.0f;
	Cost = 30;
}

bool ASHealthPotion::ApplyEffect_Implementation(APawn* InstigatorPawn)
{
	if (!Super::ApplyEffect_Implementation(InstigatorPawn))
	{
		return false;
	}
	
	if (!ensure(InstigatorPawn))
	{
		return false;
	}

	ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (!PlayerState)
	{
		return false;
	}

	if (PlayerState->GetCredits() < Cost)
	{
		return false;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AttributeComp->ApplyHealthChange(this, HealthRestoreAmount))
		{
			PlayerState->RemoveCredits(Cost);
			return true;
		}
	}

	return false;
}
