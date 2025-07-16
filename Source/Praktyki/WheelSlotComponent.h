// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WheelSlotComponent.generated.h"

class UCustomVehicleMovementComponent;
class UStaticMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PRAKTYKI_API UWheelSlotComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWheelSlotComponent();

	virtual void OnRegister() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// Params
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SpringStrenght = 1000000.0f;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SpringDamper = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SpringMaxUpDistance = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SpringDefaultDistance = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SpringMaxDownDistance = 20.0f;
	UPROPERTY(EditAnywhere, Category = "Wheel")
	float WheelRadius = 32.0f;
	UPROPERTY(EditAnywhere, Category = "Wheel")
	bool bWheelDrive = true;
	UPROPERTY(EditAnywhere, Category = "Wheel")
	bool bWheelSteering = false;
	UPROPERTY(EditAnywhere, Category = "Wheel")
	bool bWheelBrake = true;
	UPROPERTY(EditAnywhere, Category = "Wheel")
	bool bWheelHandbrake = true;
	UPROPERTY(EditAnywhere, Category = "Wheel")
	float RotateWheelDirection = 1.f;
	UPROPERTY(EditAnywhere, Category = "Friction")
	float StaticFriction = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Friction")
	float KineticFriction = 0.6f;
	UPROPERTY(EditAnywhere, Category = "Friction")
	float SideForceFactor = 60.f;
	UPROPERTY(EditAnywhere, Category = "Brake")
	float BrakeingForce = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent *WheelMesh;

	// Local Variable
	UPROPERTY(BlueprintReadOnly)
	FVector LastWheelLocation;// Wheel offset from WheelSlot location in world space [cm]
	UPROPERTY(BlueprintReadOnly)
	FVector LastWheelWorldLocation;// Wheel location in world space [cm]
	UPROPERTY(BlueprintReadOnly)
	FVector NewWheelLocation; // Wheel offset from WheelSlot location in world space [cm]
	UPROPERTY(BlueprintReadOnly)
	FVector NewWheelWorldLocation; // Wheel offset from WheelSlot location in world space [cm]
	UPROPERTY(BlueprintReadOnly)
	FVector WheelVelocity; // Wheel Velocity relative to WheelSlot [cm/s]
	UPROPERTY(BlueprintReadOnly)
	FVector WheelWorldVelocity; // [cm/s]
	UPROPERTY(BlueprintReadOnly)
	float WheelAngularVelocity; // [dgr/s]
	UPROPERTY(BlueprintReadOnly)
	FHitResult CurrentHitResult;
	UPROPERTY(BlueprintReadOnly)
	bool bSlipping;

	void SetSlipping(bool bValue);

	UFUNCTION(BlueprintImplementableEvent)
	void SlippingStarted();
	UFUNCTION(BlueprintImplementableEvent)
	void SlippingStoped();

	bool Raycast();
	void UpdateWheelLocationAndVelocity(float DeltaTime);
	void SaveNewWheelLocationAsLast();
	void RotateWheel(float DeltaTime);
	float GetFrictionFactor();

private:
	int32 DebugUpdateDelay = 1;
	int32 CurrentDebugUpdateDelay = 0;
	float WheelLength = 0.0f;
};
