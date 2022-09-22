// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);


USAttributeComponent::USAttributeComponent()
{
	Health = 100.0f;
	HealthMax = 100.0;
	bIsAlive = true;
}

bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -HealthMax);
}

bool USAttributeComponent::IsAlive() const
{
	return bIsAlive;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	if (NewHealth == Health)
	{
		return false;
	}

	Health = NewHealth;
	float ActualDelta = Health - OldHealth;
	bIsAlive = Health > 0.0f;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

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
