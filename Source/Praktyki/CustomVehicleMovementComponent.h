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

	void Setup(ACustomPlayerVehiclePawn* CustomPlayerVehiclePawn, TArray<UWheelSlotComponent*> WheelSlotComponts);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<UWheelSlotComponent*> WheelSlots;

	ACustomPlayerVehiclePawn* PlayerVehiclePawn;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	void SuspensionSpringForce();
};
