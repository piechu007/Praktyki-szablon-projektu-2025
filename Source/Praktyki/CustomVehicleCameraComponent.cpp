// Copyright 2025 Teyon. All Rights Reserved.


#include "CustomVehicleCameraComponent.h"
#include "CustomPlayerVehiclePawn.h"
#include "CustomVehicleMovementComponent.h"

void UCustomVehicleCameraComponent::BeginPlay()
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

float UCustomVehicleCameraComponent::GetForwardSpeed()
{
    if (VehicleMovementComponent != nullptr)
    {
        return VehicleMovementComponent->GetForwardVelocity();
    }
    return 0.0f;
}
