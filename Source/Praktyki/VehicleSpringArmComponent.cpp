// Copyright 2025 Teyon. All Rights Reserved.

#include "VehicleSpringArmComponent.h"
#include "Camera/CameraComponent.h"

void UVehicleSpringArmComponent::BeginPlay()
{
    Super::BeginPlay();
    StartRotation = GetRelativeRotation();
}

void UVehicleSpringArmComponent::TurnOn(UCameraComponent *CameraComponent)
{
    SetComponentTickEnabled(true);
    SetRelativeRotation(StartRotation);

    CameraComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void UVehicleSpringArmComponent::TurnOff()
{
    SetComponentTickEnabled(false);
}

void UVehicleSpringArmComponent::SetLookUpInput(float Value)
{
    SetRelativeRotation(FRotator(StartRotation.Pitch + Value * UpDownRotationScale, GetRelativeRotation().Yaw, StartRotation.Roll));
}

void UVehicleSpringArmComponent::SetLookRightInput(float Value)
{
    SetRelativeRotation(FRotator(GetRelativeRotation().Pitch, StartRotation.Yaw + Value * LeftRightRotationScale, StartRotation.Roll));
}

void UVehicleSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FRotator CurrentRotation = GetRelativeRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, StartRotation, DeltaTime, BackToDefaultRotationSpeed);

    SetRelativeRotation(NewRotation);
}
