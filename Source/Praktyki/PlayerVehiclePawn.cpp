// Copyright 2025 Teyon. All Rights Reserved.


#include "PlayerVehiclePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
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

void APlayerVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Throttle", this, &APlayerVehiclePawn::SetThrottleInput);
	PlayerInputComponent->BindAxis("Brake", this, &APlayerVehiclePawn::SetBrakeInput);
	PlayerInputComponent->BindAxis("Steering", this, &APlayerVehiclePawn::SetSteeringInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerVehiclePawn::SetLookRihtInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerVehiclePawn::SetLookUpInput);

	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AVehiclePawn::Jump);

	//PlayerInputComponent->BindAxis("MoveForward", this, &AVehiclePawn::MoveForward);
	//PlayerInputComponent->BindAxis("MoveRight", this, &AVehiclePawn::MoveRight);
	//PlayerInputComponent->BindAxis("Turn", this, &AVehiclePawn::Turn);
	//PlayerInputComponent->BindAxis("LookUp", this, &AVehiclePawn::LookUp);

	//PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AVehiclePawn::EquipButtonPressed);
	//PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AVehiclePawn::CrouchButtonPressed);
	//PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AVehiclePawn::AimButtonPressed);
	//PlayerInputComponent->BindAction("Aim", IE_Released, this, &AVehiclePawn::AimButtonReleased);
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AVehiclePawn::FireButtonPressed);
	//PlayerInputComponent->BindAction("Fire", IE_Released, this, &AVehiclePawn::FireButtonReleased);
	//PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AVehiclePawn::ReloadButtonPressed);
	//PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AVehiclePawn::GrenadeButtonPressed);
}

void APlayerVehiclePawn::SetThrottleInput(float Value)
{
    GetVehicleMovementComponent()->SetThrottleInput(Value);
}

void APlayerVehiclePawn::SetBrakeInput(float Value)
{
    GetVehicleMovementComponent()->SetBrakeInput(Value);
}

void APlayerVehiclePawn::SetSteeringInput(float Value)
{
    GetVehicleMovementComponent()->SetSteeringInput(Value);
}

void APlayerVehiclePawn::SetLookUpInput(float Value)
{
    CameraBoom->AddRelativeRotation(FRotator(Value, 0.0f, 0.0f));
}

void APlayerVehiclePawn::SetLookRihtInput(float Value)
{
    CameraBoom->AddRelativeRotation(FRotator(0.0f, Value, 0.0f));
}

void APlayerVehiclePawn::Tick(float DeltaTime)
{
	FVector VelocityVec = GetVelocity();
	float Velocity = VelocityVec.Size() * 0.036f;
	UE_LOG(LogTemp, Display, TEXT("Velocity = %f"), Velocity);
}
