// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"
#include "SAction.h"
#include "SActionComponent.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SMonsterData.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/AssetManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning bots via timer."), ECVF_Cheat); 


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	PlayerRespawnDelay = 2.0f;
	CreditsForBotKilling = 10;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000.0f;

	PlayerStateClass = ASPlayerState::StaticClass();

	SlotName = "SaveGame01";
}


void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}


void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	SpawnPowerups();
}


void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Calling Before Super:: so we set variables before 'beginplayingstat' is called in PlayerController (which is where we instantiate UI)
	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}


void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // @fixme: pass in player? for kill credit
		}
	}
}


void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Bot spawning disabled via 'CVarSpawnBots'."));
		return;
	}

	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		if (USAttributeComponent::IsActorAlive(Bot))
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %d alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void ASGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* AssetManager = UAssetManager::GetIfValid();
			if (AssetManager)
			{
				LogOnScreen(this, "Loading Monster... ", FColor::Green);
				
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				AssetManager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}
	}
}

void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId MonsterId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading.", FColor::Green);
	
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	if (AssetManager)
	{
		USMonsterData* MonsterData = Cast<USMonsterData>(AssetManager->GetPrimaryAssetObject(MonsterId));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawn enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));
	
				USActionComponent* ActionComponent = Cast<USActionComponent>(NewBot->GetComponentByClass(USActionComponent::StaticClass()));
				if (ActionComponent)
				{
					for (TSubclassOf<USAction> Action : MonsterData->Actions)
					{
						ActionComponent->AddAction(NewBot, Action);
					}
				}
			}
		}
	}
}


void ASGameModeBase::SpawnPowerups()
{
	if (PowerupClasses.Num() > 0)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}


void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn powerups EQS Query failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// Break out if reached desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}


void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	ASAICharacter* Bot = Cast<ASAICharacter>(VictimActor);
	if (Bot)
	{
		ASCharacter* Player = Cast<ASCharacter>(KillerActor);
		if (Player)
		{
			ASPlayerState* PlayerState = Player->GetPlayerState<ASPlayerState>();
			if (PlayerState)
			{
				PlayerState->AddCredits(CreditsForBotKilling);
			}
		}
	}
	else
	{
		ASCharacter* Player = Cast<ASCharacter>(VictimActor);
		if (Player)
		{
			FTimerHandle TimerHandle_RespawnDelay;

			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

			GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, PlayerRespawnDelay, false);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(KillerActor));
}


void ASGameModeBase::WriteSaveGame()
{
	for (int i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // single player only at this point
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemoryWriter(ActorData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Archive.ArIsSaveGame = true;
		// Convert Actor's SaveGame UPROPERTIES into a binary array
		Actor->Serialize(Archive);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}


void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemoryReader(ActorData.ByteData);
					
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					// Convert binary array back into Actor's variables
					Actor->Serialize(Archive);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}
