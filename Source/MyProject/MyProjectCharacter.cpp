// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AMyProjectCharacter::AMyProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMyProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Move);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}


void AMyProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraOffset(DeltaTime);
}

void AMyProjectCharacter::UpdateCameraOffset(float DeltaTime)
{
	if (!CameraBoom) return;

	const FVector PlayerLocation = GetActorLocation();
	const FVector CameraLocation = FollowCamera->GetComponentLocation();

	// Diferencia entre c�mara y personaje en XY (plano horizontal)
	const FVector2D OffsetXY = FVector2D(PlayerLocation - CameraLocation);

	// Zona segura
	FVector NewOffset = CameraBoom->TargetOffset;

	// Eje X (adelante/atr�s)
	if (FMath::Abs(OffsetXY.X) > CameraDeadZone.X)
	{
		float DeltaX = OffsetXY.X - FMath::Sign(OffsetXY.X) * CameraDeadZone.X;
		NewOffset.X = FMath::FInterpTo(NewOffset.X, DeltaX, DeltaTime, CameraLagSpeed);
	}
	else
	{
		NewOffset.X = FMath::FInterpTo(NewOffset.X, 0.0f, DeltaTime, CameraLagSpeed);
	}

	// Eje Y (izquierda/derecha)
	if (FMath::Abs(OffsetXY.Y) > CameraDeadZone.Y)
	{
		float DeltaY = OffsetXY.Y - FMath::Sign(OffsetXY.Y) * CameraDeadZone.Y;
		NewOffset.Y = FMath::FInterpTo(NewOffset.Y, DeltaY, DeltaTime, CameraLagSpeed);
	}
	else
	{
		NewOffset.Y = FMath::FInterpTo(NewOffset.Y, 0.0f, DeltaTime, CameraLagSpeed);
	}

	// Aplica el nuevo offset al SpringArm
	CameraBoom->TargetOffset = FVector(NewOffset.X, NewOffset.Y, CameraBoom->TargetOffset.Z);
}

void AMyProjectCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}
