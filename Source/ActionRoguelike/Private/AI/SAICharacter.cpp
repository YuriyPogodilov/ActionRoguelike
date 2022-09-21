// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParameterName = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	
	DrawDebugString(GetWorld(), Pawn->GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParameterName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f)
		{
			// stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			// set lifespan
			SetLifeSpan(10.f);
		}
	}
}

