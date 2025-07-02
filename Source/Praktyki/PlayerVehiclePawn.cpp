// Copyright 2025 Teyon. All Rights Reserved.

#include "PlayerVehiclePawn.h"
#include "ChaosVehicleCameraComponent.h"
#include "ChaosEngineAudioComponent.h"
#include "VehicleSpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"

APlayerVehiclePawn::APlayerVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	for (int32 i = 0; i < CameraBoomsCount; i++)
	{
		FName ComponentName = *FString::Printf(TEXT("CameraBoom_%d"), i);
		CameraBooms.Add(CreateDefaultSubobject<UVehicleSpringArmComponent>(ComponentName));
		CameraBooms[i]->SetupAttachment(GetMesh());
		CameraBooms[i]->TurnOff();
	}

	FollowCamera = CreateDefaultSubobject<UChaosVehicleCameraComponent>(TEXT("FollowCamera"));

	CurrentCameraBoomIndex = 0;
	CameraBooms[CurrentCameraBoomIndex]->TurnOn(FollowCamera);

	EngineSound = CreateDefaultSubobject<UChaosEngineAudioComponent>(TEXT("EngineSound"));
	EngineSound->SetupAttachment(GetMesh(), TEXT("EngineExtAudioSourceComponentSocket"));
}

void APlayerVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	VehiclePlayerController = Cast<APlayerController>(GetController());
	if (VehiclePlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(VehiclePlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(VehicleMapingContext, 0);
		}
	}
}

UActorComponent *APlayerVehiclePawn::CreateNewComponet(TSubclassOf<UActorComponent> ComponentClass)
{
	UActorComponent *NewComponent = NewObject<UActorComponent>(this, ComponentClass);
	if (NewComponent)
	{
		NewComponent->RegisterComponent();
		if (USceneComponent *NewSceneComponent = Cast<USceneComponent>(NewComponent))
		{
			NewSceneComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
		NewComponent->OnComponentCreated();
		NewComponent->Activate(true);
		return NewComponent;
	}

	return nullptr;
}

void APlayerVehiclePawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &APlayerVehiclePawn::SetSteeringInput);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &APlayerVehiclePawn::SetSteeringInput);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &APlayerVehiclePawn::SetThrottleInput);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &APlayerVehiclePawn::SetThrottleInput);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &APlayerVehiclePawn::SetBrakeInput);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &APlayerVehiclePawn::SetBrakeInput);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &APlayerVehiclePawn::SetHandbrakeTriggeredInput);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &APlayerVehiclePawn::SetHandbrakeCompletedInput);
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Started, this, &APlayerVehiclePawn::SetToggleCameraInput);
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &APlayerVehiclePawn::SetLookUpInput);
		EnhancedInputComponent->BindAction(LookRightAction, ETriggerEvent::Triggered, this, &APlayerVehiclePawn::SetLookRightInput);
	}
}

void APlayerVehiclePawn::SetSteeringInput(const FInputActionValue &Value)
{
	GetVehicleMovementComponent()->SetSteeringInput(Value.Get<float>());
}

void APlayerVehiclePawn::SetThrottleInput(const FInputActionValue &Value)
{
	GetVehicleMovementComponent()->SetThrottleInput(Value.Get<float>());
}

void APlayerVehiclePawn::SetBrakeInput(const FInputActionValue &Value)
{
	GetVehicleMovementComponent()->SetBrakeInput(Value.Get<float>());
}

void APlayerVehiclePawn::SetHandbrakeTriggeredInput(const FInputActionValue &Value)
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void APlayerVehiclePawn::SetHandbrakeCompletedInput(const FInputActionValue &Value)
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void APlayerVehiclePawn::SetToggleCameraInput(const FInputActionValue &Value)
{
	CameraBooms[CurrentCameraBoomIndex]->TurnOff();
	CurrentCameraBoomIndex = (CurrentCameraBoomIndex + 1) % CameraBoomsCount;
	CameraBooms[CurrentCameraBoomIndex]->TurnOn(FollowCamera);
}

void APlayerVehiclePawn::SetLookUpInput(const FInputActionValue &Value)
{
	CameraBooms[CurrentCameraBoomIndex]->SetLookUpInput(Value.Get<float>());
	// CameraBoom->AddRelativeRotation(FRotator(Value.Get<float>(), 0.0f, 0.0f));
}

void APlayerVehiclePawn::SetLookRightInput(const FInputActionValue &Value)
{
	CameraBooms[CurrentCameraBoomIndex]->SetLookRightInput(Value.Get<float>());
	// CameraBoom->AddRelativeRotation(FRotator(0.0f, Value.Get<float>(), 0.0f));
}

void APlayerVehiclePawn::SetLockHandbrakeState(bool bNewLockHandbake)
{
	bLockHandbreak = bNewLockHandbake;
	GetVehicleMovementComponent()->SetHandbrakeInput(bLockHandbreak);
}

void APlayerVehiclePawn::Tick(float DeltaTime)
{
	if (bLockHandbreak)
	{
		GetVehicleMovementComponent()->SetHandbrakeInput(true);
	}
}

UChaosWheeledVehicleMovementComponent *APlayerVehiclePawn::GetChaosWheeledVehicleMovementComponent() const
{
	return Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
}
