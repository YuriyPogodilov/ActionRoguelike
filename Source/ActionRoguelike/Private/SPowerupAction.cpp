// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupAction.h"

#include "SActionComponent.h"
#include "SAction.h"
#include "ActionRoguelike/ActionRoguelike.h"

bool ASPowerupAction::ApplyEffect_Implementation(APawn* InstigatorPawn)
{
	if (!Super::ApplyEffect_Implementation(InstigatorPawn))
	{
		return false;
	}

	if (!ensure(InstigatorPawn && ActionToPickup))
	{
		return false;	
	}
	
	if (USActionComponent* Comp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass())))
	{
		if (Comp->GetAction(ActionToPickup))
		{
			FString DebugMsg = FString::Printf(TEXT("Action %s is alreadt=y known."), *GetNameSafe(ActionToPickup));
			LogOnScreen(this, DebugMsg, FColor::Red, 2.0f);
			return false;
		}

		Comp->AddAction(InstigatorPawn, ActionToPickup);
		return true;
	}
	
	return false;
}
