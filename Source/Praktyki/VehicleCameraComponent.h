// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "VehicleCameraComponent.generated.h"


/**
 *
 */
UCLASS()
class PRAKTYKI_API UVehicleCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual float GetForwardSpeed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Field of View")
	float MinFOV = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Field of View")
	float MaxExtraFOV = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Field of View")
	float SpeedForMaxFOV = 120.f;
};
