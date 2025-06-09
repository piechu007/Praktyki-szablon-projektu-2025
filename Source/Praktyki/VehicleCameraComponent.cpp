// Copyright 2025 Teyon. All Rights Reserved.


#include "VehicleCameraComponent.h"
#include "PlayerVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void UVehicleCameraComponent::BeginPlay()
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

void UVehicleCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(!VehicleMovementComponent)
    {
        SetFieldOfView(MinFOV);
        return;
    }

    float Speed = VehicleMovementComponent->GetForwardSpeed();
    float FOVScale;
    if (Speed > SpeedForMaxFOV)
    {
        FOVScale = 1.f;
    }
    else
    {
        FOVScale = Speed/SpeedForMaxFOV;
    }
    SetFieldOfView(MinFOV + MaxExtraFOV * FOVScale);
}