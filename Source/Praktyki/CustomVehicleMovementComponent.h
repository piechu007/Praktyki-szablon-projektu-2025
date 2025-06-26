// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomVehicleMovementComponent.generated.h"

class UWheelSlotComponent;
class ACustomPlayerVehiclePawn;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PRAKTYKI_API UCustomVehicleMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCustomVehicleMovementComponent();

	void Setup(ACustomPlayerVehiclePawn *CustomPlayerVehiclePawn, TArray<UWheelSlotComponent *> WheelSlotComponts);

	void SetThrottleInput(float Input);
	void SetSteeringInput(float Input);
	void SetBrakeInput(float Input);
	void SetHandbrakeInput(bool bInput);

	UPROPERTY(EditAnywhere, Category = "Engine")
	float EngineForce = 10000.f;
	UPROPERTY(EditAnywhere, Category = "Engine")
	float EngineBrakingForce = 1000.f;
	UPROPERTY(EditAnywhere, Category = "TurningAngle")
	float TurningAngle = 60.f;

protected:

	UPROPERTY(BlueprintReadOnly)
	float ThrottleInput = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float SteeringInput = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float BrakeInput = 0.f;
	UPROPERTY(BlueprintReadOnly)
	bool HandbrakeInput = false;

	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<UWheelSlotComponent *> WheelSlots;

	ACustomPlayerVehiclePawn *PlayerVehiclePawn;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	void AddForcesFromAllWheels(float DeltaTime);
	void UpdateAngleOfSteeringWheels();
	FVector GetForceFromWheel(UWheelSlotComponent* WheelSlot, float DeltaTime);

	void SuspensionSpringForce();
};
