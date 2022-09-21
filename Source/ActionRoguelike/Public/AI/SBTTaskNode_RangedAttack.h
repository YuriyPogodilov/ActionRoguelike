// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTaskNode_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTTaskNode_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:

	USBTTaskNode_RangedAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;
};
