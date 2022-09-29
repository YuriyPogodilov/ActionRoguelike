// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Blackhole.h"

#include "SAttributeComponent.h"


USAction_Blackhole::USAction_Blackhole()
{
	ActionName = "BlackHole";
	RageCost = 50.0f;
}

bool USAction_Blackhole::CanStart_Implementation(AActor* Instigator) const
{
	if (!Super::CanStart_Implementation(Instigator))
	{
		return false;
	}
	
	USAttributeComponent* Comp = USAttributeComponent::GetAttributes(Instigator); 
	if (Comp)
	{ 
		return Comp->GetRage() >= RageCost;
	}
		
	return false;
}

void USAction_Blackhole::StartAction_Implementation(AActor* Instigator)
{
	USAttributeComponent* Comp = USAttributeComponent::GetAttributes(Instigator); 
	if (!ensure(Comp && Comp->ApplyRageChange(Instigator, -RageCost)))
	{
		return;
	}
	
	Super::StartAction_Implementation(Instigator);
}
