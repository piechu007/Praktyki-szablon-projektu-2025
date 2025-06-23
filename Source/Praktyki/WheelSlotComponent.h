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

	FVector GetForce(float DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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


private:
	bool Raycast(FHitResult& OutHit);

	void UpdateWheelLocationAndVelocity(FHitResult& HitResult, float DeltaTime);

	FVector GetSuspertionForce(FHitResult& HitResult, float DeltaTime);
	FVector GetSideForce(FHitResult& HitResult, float DeltaTime);
	FVector GetForwardForce(FHitResult& HitResult, float DeltaTime);

	FVector LastWheelLocation;
	FVector NewWheelLocation;
	FVector WheelVelocity;
};
