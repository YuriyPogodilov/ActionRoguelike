// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"



void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}


void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.InstigatorActor = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}
	
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}


void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	// ensureAlways(bIsRunning);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.InstigatorActor = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}


UWorld* USAction::GetWorld() const
{
	// Outer is set when creating NewObject<Ts>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}


USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComp;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.InstigatorActor);
	}
	else
	{
		StopAction(RepData.InstigatorActor);
	}
}


bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}


bool USAction::CanStart_Implementation(AActor* Instigator) const
{
	return !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags) && !IsRunning();
}


void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
}
