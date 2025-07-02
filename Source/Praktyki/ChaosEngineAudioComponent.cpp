// Copyright 2025 Teyon. All Rights Reserved.

#include "ChaosEngineAudioComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "PlayerVehiclePawn.h"

void UChaosEngineAudioComponent::BeginPlay()
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
}

int32 UChaosEngineAudioComponent::GetCurrentGear()
{
    if (VehicleMovementComponent != nullptr)
    {
        return VehicleMovementComponent->GetCurrentGear();
    }
    return 0;
}

float UChaosEngineAudioComponent::GetCurrentRPM()
{
    if (VehicleMovementComponent != nullptr)
    {
        return VehicleMovementComponent->GetEngineRotationSpeed();
    }
    return 0.0f;
}
