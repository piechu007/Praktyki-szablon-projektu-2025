// Copyright 2025 Teyon. All Rights Reserved.


#include "CustomEngineAudioComponent.h"
#include "CustomPlayerVehiclePawn.h"
#include "CustomVehicleMovementComponent.h"

void UCustomEngineAudioComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor *Owner = GetOwner())
    {
        if (ACustomPlayerVehiclePawn *PlayerVehiclePawn = Cast<ACustomPlayerVehiclePawn>(Owner))
        {
            if (UCustomVehicleMovementComponent *CustomVehicleMovementComponent = PlayerVehiclePawn->GetCustomVehicleMovementComponent())
            {
                VehicleMovementComponent = CustomVehicleMovementComponent;
            }
        }
    }
}

int32 UCustomEngineAudioComponent::GetCurrentGear()
{
    if (VehicleMovementComponent != nullptr)
    {
        return VehicleMovementComponent->GetCurrentGear();
    }
    return 0;
}

float UCustomEngineAudioComponent::GetCurrentRPM()
{
    if (VehicleMovementComponent != nullptr)
    {
        return VehicleMovementComponent->GetCurrentRPM();
    }
    return 0.0f;
}

