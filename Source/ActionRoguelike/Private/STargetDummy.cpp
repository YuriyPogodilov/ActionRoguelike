// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

#include "SActionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttribComp");
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
}

void ASTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HealthBarWidgetClass)
	{
		ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
		}
	}
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
