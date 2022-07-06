// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickableActorBase.h"

ASPickableActorBase::ASPickableActorBase()
{
	MeshBase = CreateDefaultSubobject<UStaticMeshComponent>("MeshBase");
	RootComponent = MeshBase;
}

void ASPickableActorBase::SetActive(bool bNewIsActive)
{
	if (bIsActive == bNewIsActive)
	{
		return;
	}

	bIsActive = bNewIsActive;
	RootComponent->SetVisibility(bIsActive, true);
	SetActorEnableCollision(bIsActive);
}

void ASPickableActorBase::ShowUp()
{
	SetActive(true);
}

void ASPickableActorBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ApplyEffect(InstigatorPawn))
	{
		SetActive(false);

		GetWorldTimerManager().SetTimer(CooldownTimer, this, &ASPickableActorBase::ShowUp, CooldownTime); 
	}
	
}

bool ASPickableActorBase::ApplyEffect_Implementation(APawn* InstigatorPawn)
{
	return false;
}
