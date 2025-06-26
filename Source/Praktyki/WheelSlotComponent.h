// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WheelSlotComponent.generated.h"

class UCustomVehicleMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRAKTYKI_API UWheelSlotComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWheelSlotComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
	UPROPERTY(EditAnywhere, Category = "Friction") 
	float StaticFriction = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Friction") 
	float KineticFriction = 0.6f;
	UPROPERTY(EditAnywhere, Category = "Brake") 
	float BrakeingForce = 10000.f;

	// Local Variable
	UPROPERTY(BlueprintReadOnly) 
	FVector LastWheelLocation;
	UPROPERTY(BlueprintReadOnly) 
	FVector LastWheelWorldLocation;
	UPROPERTY(BlueprintReadOnly) 
	FVector NewWheelLocation;
	UPROPERTY(BlueprintReadOnly) 
	FVector NewWheelWorldLocation;
	UPROPERTY(BlueprintReadOnly) 
	FVector WheelVelocity;
	UPROPERTY(BlueprintReadOnly) 
	FVector WheelWorldVelocity;
	UPROPERTY(BlueprintReadOnly) 
	FHitResult CurrentHitResult;
	UPROPERTY(BlueprintReadOnly) 
	bool bSlipping;

	bool Raycast();
	void UpdateWheelLocationAndVelocity(float DeltaTime);
	void SaveNewWheelLocationAsLast();
	FVector GetForce(float DeltaTime);

	FVector GetSuspertionForce(float DeltaTime);
	FVector GetSideForce(float DeltaTime);
	FVector GetForwardForce(float DeltaTime);
};
