// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	Health = 100.0f;
	MaxHealth = 100.0;
	bIsAlive = true;
}

bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -MaxHealth);
}

bool USAttributeComponent::IsAlive() const
{
	return bIsAlive;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	if (NewHealth == Health)
	{
		return false;
	}

	Health = NewHealth;
	float ActualDelta = Health - OldHealth;
	bIsAlive = Health > 0.0f;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return true;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* Attributes = GetAttributes(Actor);
	if (Attributes)
	{
		return Attributes->IsAlive();
	}

	return false;
}
