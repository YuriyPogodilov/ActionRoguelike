// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"

bool ASHealthPotion::ApplyEffect_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (IsValid(AttributeComp))
	{
		return AttributeComp->ApplyHealthChange(HealthRestoreAmount);
	}

	return false;
}
