// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	ActionName = "Thorns";
	
	Duration = 0.0f;

	ReflectFraction = 0.2f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributeComponent* Comp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (ensure(Comp))
	{
		Comp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::ReflectDamage);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* Comp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (ensure(Comp))
	{
		Comp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::ReflectDamage);
	}
}

void USActionEffect_Thorns::ReflectDamage(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta > 0.0f)
	{
		return;
	}

	if (InstigatorActor == OwningComp->GetOwner())
	{
		return;
	}

	float ReflectedDamage = FMath::Abs(FMath::RoundToInt(Delta * ReflectFraction));
	USGameplayFunctionLibrary::ApplyDamage(OwningComp->GetOwner(), InstigatorActor, ReflectedDamage);
}
