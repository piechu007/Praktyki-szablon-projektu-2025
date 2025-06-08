// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TestOutOfTrackComponent.generated.h"

class UChaosWheeledVehicleMovementComponent;
class AWheeledVehiclePawn;
struct FWheelStatus;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOutOfTrack, AWheeledVehiclePawn *, OutOfTrackVehiclePawn);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PRAKTYKI_API UTestOutOfTrackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTestOutOfTrackComponent();

	UPROPERTY(BlueprintAssignable)
	FOnOutOfTrack OnOutOfTrack;

protected:
	virtual void OnComponentCreated() override;

	// Called when the game starts
	virtual void BeginPlay() override;

	UChaosWheeledVehicleMovementComponent *VehicleMovementComponent;
	AWheeledVehiclePawn *VehiclePawn;

	TArray<FString> NoTrackPhysicMaterialNames;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	bool CheckAllWheelOutOfTrack() const;
	bool CheckWheelOutOfTrack(FWheelStatus& WheelStatus) const;
};
