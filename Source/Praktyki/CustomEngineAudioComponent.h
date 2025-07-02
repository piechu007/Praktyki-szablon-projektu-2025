// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineAudioComponent.h"
#include "CustomEngineAudioComponent.generated.h"

class UCustomVehicleMovementComponent;

/**
 *
 */
UCLASS()
class PRAKTYKI_API UCustomEngineAudioComponent : public UEngineAudioComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	virtual int32 GetCurrentGear() override;
	virtual float GetCurrentRPM() override;

	UCustomVehicleMovementComponent *VehicleMovementComponent;
};
