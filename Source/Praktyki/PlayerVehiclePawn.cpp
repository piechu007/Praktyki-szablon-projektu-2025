// Copyright 2025 Teyon. All Rights Reserved.


#include "PlayerVehiclePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ChaosVehicleMovementComponent.h"

APlayerVehiclePawn::APlayerVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	//CameraBoom->bUsePawnControlRotation = true;
    
	UCameraComponent* FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//FollowCamera->bUsePawnControlRotation = false;

}

void APlayerVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	// Setup TankMapingContext
    VehiclePlayerController = Cast<APlayerController>(GetController());
    if (VehiclePlayerController)
    {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(VehiclePlayerController->GetLocalPlayer());
        if (Subsystem)
        {
            Subsystem->AddMappingContext(VehicleMapingContext, 0);
        }
    }
}


void APlayerVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent)
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
    // TODO: ToggleCamera
}

void APlayerVehiclePawn::SetLookUpInput(const FInputActionValue &Value)
{
    CameraBoom->AddRelativeRotation(FRotator(Value.Get<float>(), 0.0f, 0.0f));
}

void APlayerVehiclePawn::SetLookRightInput(const FInputActionValue &Value)
{
    CameraBoom->AddRelativeRotation(FRotator(0.0f, Value.Get<float>(), 0.0f));
}

void APlayerVehiclePawn::Tick(float DeltaTime)
{
	//FVector VelocityVec = GetVelocity();
	//float Velocity = VelocityVec.Size() * 0.036f;
	//UE_LOG(LogTemp, Display, TEXT("Velocity = %f"), Velocity);
}
