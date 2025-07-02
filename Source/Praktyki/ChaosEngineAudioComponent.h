// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineAudioComponent.h"
#include "ChaosEngineAudioComponent.generated.h"

class UChaosWheeledVehicleMovementComponent;

/**
 *
 */
UCLASS()
class PRAKTYKI_API UChaosEngineAudioComponent : public UEngineAudioComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	virtual int32 GetCurrentGear() override;
	virtual float GetCurrentRPM() override;

	UChaosWheeledVehicleMovementComponent *VehicleMovementComponent;
};
