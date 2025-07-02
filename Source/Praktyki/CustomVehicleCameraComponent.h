// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VehicleCameraComponent.h"
#include "CustomVehicleCameraComponent.generated.h"

class UCustomVehicleMovementComponent;

/**
 *
 */
UCLASS()
class PRAKTYKI_API UCustomVehicleCameraComponent : public UVehicleCameraComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	virtual float GetForwardSpeed() override;

	UCustomVehicleMovementComponent *VehicleMovementComponent;
};
