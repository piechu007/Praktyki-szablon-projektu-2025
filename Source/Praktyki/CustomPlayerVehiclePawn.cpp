// Copyright 2025 Teyon. All Rights Reserved.

#include "CustomPlayerVehiclePawn.h"
#include "VehicleCameraComponent.h"
#include "EngineAudioComponent.h"
#include "VehicleSpringArmComponent.h"
#include "WheelSlotComponent.h"
#include "CustomVehicleMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

ACustomPlayerVehiclePawn::ACustomPlayerVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Movment
	VehicleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vehicle Mesh"));
	RootComponent = VehicleMesh;
	VehicleMovement = CreateDefaultSubobject<UCustomVehicleMovementComponent>(TEXT("Vehicle Movement"));

	// Cameras
	for (int32 i = 0; i < CameraBoomsCount; i++)
	{
		FName ComponentName = *FString::Printf(TEXT("CameraBoom_%d"), i);
		CameraBooms.Add(CreateDefaultSubobject<UVehicleSpringArmComponent>(ComponentName));
		CameraBooms[i]->SetupAttachment(VehicleMesh);
		CameraBooms[i]->TurnOff();
	}

	FollowCamera = CreateDefaultSubobject<UVehicleCameraComponent>(TEXT("FollowCamera"));

	CurrentCameraBoomIndex = 0;
	CameraBooms[CurrentCameraBoomIndex]->TurnOn(FollowCamera);

	// Sounds
	EngineSound = CreateDefaultSubobject<UEngineAudioComponent>(TEXT("EngineSound"));
	EngineSound->SetupAttachment(VehicleMesh, TEXT("EngineExtAudioSourceComponentSocket"));
}

void ACustomPlayerVehiclePawn::BeginPlay()
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

	//WheelSlotComponents = GetComponentsByClass(UWheelSlotComponent::StaticClass());
	GetComponents<UWheelSlotComponent>(WheelSlotComponents);
	VehicleMovement->Setup(this, WheelSlotComponents);
}

UActorComponent *ACustomPlayerVehiclePawn::CreateNewComponet(TSubclassOf<UActorComponent> ComponentClass)
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

void ACustomPlayerVehiclePawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &ACustomPlayerVehiclePawn::SetSteeringInput);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &ACustomPlayerVehiclePawn::SetSteeringInput);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &ACustomPlayerVehiclePawn::SetThrottleInput);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &ACustomPlayerVehiclePawn::SetThrottleInput);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ACustomPlayerVehiclePawn::SetBrakeInput);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ACustomPlayerVehiclePawn::SetBrakeInput);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &ACustomPlayerVehiclePawn::SetHandbrakeTriggeredInput);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ACustomPlayerVehiclePawn::SetHandbrakeCompletedInput);
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Started, this, &ACustomPlayerVehiclePawn::SetToggleCameraInput);
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ACustomPlayerVehiclePawn::SetLookUpInput);
		EnhancedInputComponent->BindAction(LookRightAction, ETriggerEvent::Triggered, this, &ACustomPlayerVehiclePawn::SetLookRightInput);
	}
}

void ACustomPlayerVehiclePawn::SetSteeringInput(const FInputActionValue &Value)
{
	// GetVehicleMovementComponent()->SetSteeringInput(Value.Get<float>());// TODO: refactor
}

void ACustomPlayerVehiclePawn::SetThrottleInput(const FInputActionValue &Value)
{
	// GetVehicleMovementComponent()->SetThrottleInput(Value.Get<float>());// TODO: refactor
}

void ACustomPlayerVehiclePawn::SetBrakeInput(const FInputActionValue &Value)
{
	// GetVehicleMovementComponent()->SetBrakeInput(Value.Get<float>());// TODO: refactor
}

void ACustomPlayerVehiclePawn::SetHandbrakeTriggeredInput(const FInputActionValue &Value)
{
	// GetVehicleMovementComponent()->SetHandbrakeInput(true);// TODO: refactor
}

void ACustomPlayerVehiclePawn::SetHandbrakeCompletedInput(const FInputActionValue &Value)
{
	// GetVehicleMovementComponent()->SetHandbrakeInput(false);// TODO: refactor
}

void ACustomPlayerVehiclePawn::SetToggleCameraInput(const FInputActionValue &Value)
{
	CameraBooms[CurrentCameraBoomIndex]->TurnOff();
	CurrentCameraBoomIndex = (CurrentCameraBoomIndex + 1) % CameraBoomsCount;
	CameraBooms[CurrentCameraBoomIndex]->TurnOn(FollowCamera);
}

void ACustomPlayerVehiclePawn::SetLookUpInput(const FInputActionValue &Value)
{
	CameraBooms[CurrentCameraBoomIndex]->SetLookUpInput(Value.Get<float>());
	// CameraBoom->AddRelativeRotation(FRotator(Value.Get<float>(), 0.0f, 0.0f));
}

void ACustomPlayerVehiclePawn::SetLookRightInput(const FInputActionValue &Value)
{
	CameraBooms[CurrentCameraBoomIndex]->SetLookRightInput(Value.Get<float>());
	// CameraBoom->AddRelativeRotation(FRotator(0.0f, Value.Get<float>(), 0.0f));
}

void ACustomPlayerVehiclePawn::SetLockHandbrakeState(bool bNewLockHandbake)
{
	bLockHandbreak = bNewLockHandbake;
	// GetVehicleMovementComponent()->SetHandbrakeInput(bLockHandbreak);  // TODO: refactor
}

void ACustomPlayerVehiclePawn::Tick(float DeltaTime)
{
	if (bLockHandbreak)
	{
		// GetVehicleMovementComponent()->SetHandbrakeInput(true); // TODO: refactor
	}
}

UChaosWheeledVehicleMovementComponent *ACustomPlayerVehiclePawn::GetChaosWheeledVehicleMovementComponent() const
{
	return nullptr;
	// return Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()); // TODO: refactor
}

USkeletalMeshComponent *ACustomPlayerVehiclePawn::GetMesh()
{
	return VehicleMesh;
}
