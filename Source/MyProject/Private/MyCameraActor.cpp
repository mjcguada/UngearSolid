// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraActor.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AMyCameraActor::AMyCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AMyCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetActor)
	{
		return;
	}

	FollowTarget(DeltaTime);
}

void AMyCameraActor::SetTarget(AActor* Target)
{
	TargetActor = Target;

	if (TargetActor)
	{
		FVector StartingPosition = TargetActor->GetActorLocation();
		SetActorLocation(StartingPosition);

		// Camera Offset and Pitch
		FollowCamera->SetRelativeRotation(FRotator(PitchOffset, 0.0f, 0.0f));
		FollowCamera->SetRelativeLocation(CameraOffset);
	}
}

void AMyCameraActor::FollowTarget(float DeltaTime)
{
	FVector DesiredLocation = GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	FVector Offset = TargetLocation - GetActorLocation();

	if (FMath::Abs(Offset.X) > SafeArea.X)
	{
		DesiredLocation.X += Offset.X - FMath::Sign(Offset.X) * SafeArea.X;
	}

	if (FMath::Abs(Offset.Y) > SafeArea.Y)
	{
		DesiredLocation.Y += Offset.Y - FMath::Sign(Offset.Y) * SafeArea.Y;
	}

	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), DesiredLocation, DeltaTime, CameraSpeed);
	SetActorLocation(NewLocation);
}

