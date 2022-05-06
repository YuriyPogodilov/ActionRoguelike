// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionProfileName("PhysicsActor");
	RootComponent = StaticMesh;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>("Radial Force");
	RadialForce->bImpulseVelChange = true;
	RadialForce->SetupAttachment(StaticMesh);
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& HitResult)
{
	Explode();
}

void ASExplosiveBarrel::Explode()
{
	RadialForce->FireImpulse();
}
