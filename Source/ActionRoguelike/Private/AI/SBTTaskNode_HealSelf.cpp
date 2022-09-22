// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskNode_HealSelf.h"

#include "AIController.h"
#include "SAttributeComponent.h"




EBTNodeResult::Type USBTTaskNode_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MyPawn)
	{
		return EBTNodeResult::Failed;
	}

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);
	if (ensure(AttributeComp))
	{
		if (AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax()))
		{
			return EBTNodeResult::Succeeded;
		}
		
	}

	return EBTNodeResult::Failed;
}
