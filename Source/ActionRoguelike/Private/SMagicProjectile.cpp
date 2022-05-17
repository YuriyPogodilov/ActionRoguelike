// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "Kismet/GameplayStatics.h"

ASMagicProjectile::ASMagicProjectile()
{
	ImpactCameraShake = nullptr;
}

void ASMagicProjectile::Explode_Implementation()
{
	Super::Explode_Implementation();

	if (IsValid(ImpactCameraShake))
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactCameraShake, GetActorLocation(), 100.f, 500.0f);
	}
}
