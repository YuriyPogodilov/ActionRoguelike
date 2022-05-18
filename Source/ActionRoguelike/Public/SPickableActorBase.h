// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickableActorBase.generated.h"

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASPickableActorBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ASPickableActorBase();
};
