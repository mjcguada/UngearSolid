// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "MyCameraActor.generated.h"

UCLASS()
class MYPROJECT_API AMyCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyCameraActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTarget(AActor* Target);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCamera")
	FVector2D SafeArea = FVector2D(200.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCamera")
	FVector CameraOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCamera")
	float PitchOffset = -60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCamera")
	float CameraSpeed = 5.0f;

private:
	void FollowTarget(float DeltaTime);

	AActor* TargetActor;
};
