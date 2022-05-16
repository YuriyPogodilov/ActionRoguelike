// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	Health = 100.0f;
	MaxHealth = 100.0;
	bIsAlive = true;
}

bool USAttributeComponent::IsAlive() const
{
	return bIsAlive;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	if (NewHealth == Health)
	{
		return false;
	}

	Health = NewHealth;

	bIsAlive = Health > 0.0f;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

