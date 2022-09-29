// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupAction.h"

#include "SActionComponent.h"
#include "SAction.h"

bool ASPowerupAction::ApplyEffect_Implementation(APawn* InstigatorPawn)
{
	if (! ensure(InstigatorPawn && ActionToPickup))
	{
		return false;	
	}
	
	if (USActionComponent* Comp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass())))
	{
		if (Comp->GetAction(ActionToPickup))
		{
			FString DebugMsg = FString::Printf(TEXT("Action %s is alreadt=y known."), *GetNameSafe(ActionToPickup));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return false;
		}

		Comp->AddAction(InstigatorPawn, ActionToPickup);
		return true;
	}
	
	return false;
}