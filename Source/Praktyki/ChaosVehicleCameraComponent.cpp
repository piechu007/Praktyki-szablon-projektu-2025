// Copyright 2025 Teyon. All Rights Reserved.


#include "ChaosVehicleCameraComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "PlayerVehiclePawn.h"

void UChaosVehicleCameraComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor *Owner = GetOwner())
    {
        if (APlayerVehiclePawn *PlayerVehiclePawn = Cast<APlayerVehiclePawn>(Owner))
        {
            if (UChaosWheeledVehicleMovementComponent *ChaosWheeledVehicleMovementComponent = PlayerVehiclePawn->GetChaosWheeledVehicleMovementComponent())
            {
                VehicleMovementComponent = ChaosWheeledVehicleMovementComponent;
            }
        }
    }
    
    SpeedForMaxFOV = SpeedForMaxFOV * 100000.f / 3600.f; // km/h to cm/s
}

float UChaosVehicleCameraComponent::GetForwardSpeed()
{
    if (VehicleMovementComponent != nullptr)
    {
        return VehicleMovementComponent->GetForwardSpeed();
    }
    return 0.0f;
}
