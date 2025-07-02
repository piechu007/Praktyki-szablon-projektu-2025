// Copyright 2025 Teyon. All Rights Reserved.


#include "VehicleCameraComponent.h"
#include "PlayerVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void UVehicleCameraComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UVehicleCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    float FOVScale = GetForwardSpeed()/SpeedForMaxFOV;
    FOVScale = FMath::Clamp(FOVScale, 0.f, 1.f);
    SetFieldOfView(MinFOV + MaxExtraFOV * FOVScale);
}

float UVehicleCameraComponent::GetForwardSpeed()
{
    return 0.0f;
}

