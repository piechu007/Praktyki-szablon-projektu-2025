// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "EngineAudioComponent.generated.h"

class USoundBase;
class UChaosWheeledVehicleMovementComponent;

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PRAKTYKI_API UEngineAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	UEngineAudioComponent();

	virtual void BeginPlay() override;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void TryPlayGearShiftSound();
	void UpdateEngineSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Sound")
	float MinVolume = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Sound")
	float MinPitch = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Sound")
	float MaxExtraVolume = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Sound")
	float MaxExtraPitch = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Sound")
	float RPMForMaxPithAndVolume = 9000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Sound")
	float RPMForMinPithAndVolume = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine Sound")
    USoundBase* ShiftSound;

	UChaosWheeledVehicleMovementComponent *VehicleMovementComponent;

	int32 LastGear;
};
