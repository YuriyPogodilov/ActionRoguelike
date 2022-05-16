// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASProjectileBase::ASProjectileBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnActorOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->InitialSpeed = 8000.f;

	DamageAmount = 20.0f;
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult)
{
	Explode();
}

void ASProjectileBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-DamageAmount);

			Explode();
		}
	}
}

void ASProjectileBase::Explode_Implementation()
{
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation(), GetActorRotation());
		Destroy();
	}
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

