// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickableActorBase.h"

ASPickableActorBase::ASPickableActorBase()
{
	MeshBase = CreateDefaultSubobject<UStaticMeshComponent>("MeshBase");
	RootComponent = MeshBase;
}

void ASPickableActorBase::Activate()
{
	if (bIsActive)
	{
		return;
	}

	bIsActive = true;
	MeshBase->SetVisibility(true, true);
	MeshBase->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASPickableActorBase::Deactivate()
{
	if (!bIsActive)
	{
		return;
	}

	bIsActive = false;
	MeshBase->SetVisibility(false, true);
	MeshBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASPickableActorBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ApplyEffect(InstigatorPawn))
	{
		Deactivate();

		GetWorldTimerManager().SetTimer(CooldownTimer, this, &ASPickableActorBase::Activate, CooldownTime); 
	}
	
}

bool ASPickableActorBase::ApplyEffect_Implementation(APawn* InstigatorPawn)
{
	return false;
}
