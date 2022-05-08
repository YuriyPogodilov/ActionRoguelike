// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASDashProjectile::Explode, ExplodeDelay);
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &ASDashProjectile::OnActorHit);
}

void ASDashProjectile::Explode()
{
	MovementComp->StopMovementImmediately();
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation(), GetActorRotation());
	
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASDashProjectile::Teleport, TeleportDelay);
}

void ASDashProjectile::Teleport()
{
	GetInstigator()->TeleportTo(GetActorLocation(), GetActorRotation());
	Destroy();
}

void ASDashProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult)
{
	Explode();
}

