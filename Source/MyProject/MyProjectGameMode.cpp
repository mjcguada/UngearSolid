// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	// stub
}

void AMyProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	//if (!World || !PlayerCharacterClass || !CameraActorClass) 
	if (!World || !PlayerCharacterClass) 
	{
		return;	
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Find PlayerStarts Spawn Point
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	FVector SpawnPosition = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	if (PlayerStarts.Num() > 0)
	{
		SpawnPosition = PlayerStarts[0]->GetActorLocation();
		SpawnRotation = PlayerStarts[0]->GetActorRotation();
	}

	// Spawn the character
	APawn* MyCharacter = World->SpawnActor<APawn>(PlayerCharacterClass, SpawnPosition, SpawnRotation, SpawnParams);

	// Asign PlayerController
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && MyCharacter)
	{
		PC->Possess(MyCharacter);
	}

	// Spawn Camera
	AMyCameraActor* MyCamera = World->SpawnActor<AMyCameraActor>(CameraActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
	// Activate Camera
	PC->SetViewTarget(MyCamera);

	// Asign target
	if (MyCamera && MyCharacter)
	{
		MyCamera->SetTarget(MyCharacter);		
	}
}
