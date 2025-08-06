// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraActor.h"

// Sets default values
AMyCameraActor::AMyCameraActor()
{
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

	//FollowTarget()

}

void AMyCameraActor::SetTarget(AActor* Target)
{
	TargetActor = Target;

	PrimaryActorTick.bCanEverTick = true;
}

