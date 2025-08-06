// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AMyProjectGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<APawn> PlayerCharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AMyCameraActor> CameraActorClass;
};



