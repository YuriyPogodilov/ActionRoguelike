// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickableActorBase.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

ASPickableActorBase::ASPickableActorBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComp;

	MeshBase = CreateDefaultSubobject<UStaticMeshComponent>("MeshBase");
	MeshBase->SetupAttachment(SphereComp);

	CooldownTime = 10.0f;
	bIsActive = true;
	
	bReplicates = true;
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

FText ASPickableActorBase::GetInteractMessage_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ASPickableActorBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!HasAuthority())
	{
		return;
	}
	
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (ApplyEffect(InstigatorPawn))
	{
		SetActive(false);

		GetWorldTimerManager().SetTimer(CooldownTimer, this, &ASPickableActorBase::ShowUp, CooldownTime); 
	}
}

void ASPickableActorBase::OnRep_IsActiveChanged()
{
	RootComponent->SetVisibility(bIsActive, true);
	SetActorEnableCollision(bIsActive);	
}

bool ASPickableActorBase::ApplyEffect_Implementation(APawn* InstigatorPawn)
{
	return HasAuthority();
}

void ASPickableActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPickableActorBase, bIsActive);
}