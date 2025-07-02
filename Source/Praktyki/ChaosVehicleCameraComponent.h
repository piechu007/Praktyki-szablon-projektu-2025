// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VehicleCameraComponent.h"
#include "ChaosVehicleCameraComponent.generated.h"

class UChaosWheeledVehicleMovementComponent;

/**
 *
 */
UCLASS()
class PRAKTYKI_API UChaosVehicleCameraComponent : public UVehicleCameraComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	virtual float GetForwardSpeed() override;

	UChaosWheeledVehicleMovementComponent *VehicleMovementComponent;
};
