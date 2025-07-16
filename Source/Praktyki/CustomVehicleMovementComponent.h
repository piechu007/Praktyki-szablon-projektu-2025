// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "CustomVehicleMovementComponent.generated.h"

class UWheelSlotComponent;
class ACustomPlayerVehiclePawn;

UCLASS(EditInlineNew, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	UFUNCTION(BlueprintPure)
	float GetForwardVelocity() const;
	UFUNCTION(BlueprintPure)
	int32 GetCurrentGear() const;
	UFUNCTION(BlueprintPure)
	float GetCurrentRPM() const;

	UPROPERTY(EditAnywhere, Category = "Engine")
	float EngineMaxTorque = 600.f;
	UPROPERTY(EditAnywhere, Category = "Engine")
	float EngineBrakingForce = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Engine")
	UCurveFloat *ThrottleCurve;
	UPROPERTY(EditAnywhere, Category = "Engine")
	float MaxRPM = 9000.f;
	UPROPERTY(EditAnywhere, Category = "Engine")
	float IdleRPM = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Engine")
	float ChangeUpRPM = 7000.f;
	UPROPERTY(EditAnywhere, Category = "Engine")
	float ChangeDownRPM = 2000.f;
	UPROPERTY(EditAnywhere, Category = "Engine")
	TArray<float> ForwardGearsRatio;
	UPROPERTY(EditAnywhere, Category = "Engine")
	TArray<float> BackGearsRatio;
	UPROPERTY(EditAnywhere, Category = "Steering")
	float DefaultSteeringAngle = 60.f;
	UPROPERTY(EditAnywhere, Category = "Steering")
	UCurveFloat *SteeringCurve;
	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float DownforceFactor = 20.f;
	UPROPERTY(EditAnywhere, Category = "Vehicle")
	FVector DownforceOffset;
	UPROPERTY(EditAnywhere, Category = "Vehicle")
	float AirDragForceFactor = 32.f;
	UPROPERTY(EditAnywhere, Category = "Vehicle")
	FVector AirDragForceOffset;

protected:
	int32 CurrentGear = 0;
	int32 LastGear = 0;
	float CurrentRPM = 0.f;
	float CurrentGearRatio = 0.f;
	float CurrentEngineTorque = 0.f;
	float ForwardVelocity = 0.f;

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
	FVector GetForceFromWheel(UWheelSlotComponent *WheelSlot, float DeltaTime);

	void UpdateSpeed();
	void UpdateEngine();
	void TryAutomaticlyChangeGear();
	void ChangeGear(int32 dir);

	void AddDownforce();
	void AddAirDragForce();

	void SuspensionSpringForce();
};
