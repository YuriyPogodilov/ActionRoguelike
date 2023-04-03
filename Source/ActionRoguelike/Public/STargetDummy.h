// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

class USWorldUserWidget;
class USAction;
class USActionComponent;
class USAttributeComponent;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTargetDummy();

	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere)
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
