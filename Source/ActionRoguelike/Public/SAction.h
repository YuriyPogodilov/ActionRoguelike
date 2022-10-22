// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class USActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* InstigatorActor;
};

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated)
	USActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Tags")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

public:

	/* Action nickname to start/stop without reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UPROPERTY(Replicated)
	float TimeStarted;

	void Initialize(USActionComponent* NewActionComp);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
