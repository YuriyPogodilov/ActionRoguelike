// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupAction.h"

#include "SActionComponent.h"
#include "SAction.h"

bool ASPowerupAction::ApplyEffect_Implementation(APawn* InstigatorPawn)
{
	if (USActionComponent* Comp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass())))
	{
		if (!Comp->HasAction(ActionToPickup))
		{
			Comp->AddAction(InstigatorPawn, ActionToPickup);
			return true;
		}
	}
	
	return false;
}
