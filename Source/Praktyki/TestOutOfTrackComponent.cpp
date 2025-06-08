// Copyright 2025 Teyon. All Rights Reserved.

#include "TestOutOfTrackComponent.h"
#include "PlayerVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values for this component's properties
UTestOutOfTrackComponent::UTestOutOfTrackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTestOutOfTrackComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (AActor *Owner = GetOwner())
	{
		if (APlayerVehiclePawn *PlayerVehiclePawn = Cast<APlayerVehiclePawn>(Owner))
		{
			VehiclePawn = PlayerVehiclePawn;
			if (UChaosWheeledVehicleMovementComponent *ChaosWheeledVehicleMovementComponent = PlayerVehiclePawn->GetChaosWheeledVehicleMovementComponent())
			{
				VehicleMovementComponent = ChaosWheeledVehicleMovementComponent;
			}
		}
	}
}

// Called when the game starts
void UTestOutOfTrackComponent::BeginPlay()
{
	Super::BeginPlay();

	NoTrackPhysicMaterialNames = {"PM_Grass"};
}

// Called every frame
void UTestOutOfTrackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CheckAllWheelOutOfTrack())
	{
		OnOutOfTrack.Broadcast(VehiclePawn);
	}
}

bool UTestOutOfTrackComponent::CheckAllWheelOutOfTrack() const
{
	if (VehicleMovementComponent)
	{
		int32 NumWheels = VehicleMovementComponent->WheelSetups.Num();
		int32 NumWheelsOutOfTrack = 0;

		for (int32 i = 0; i < NumWheels; ++i)
		{
			FWheelStatus WheelStatus = (VehicleMovementComponent->GetWheelState(i));
			if(CheckWheelOutOfTrack(WheelStatus))
			{
				NumWheelsOutOfTrack++;
			}
		}

		if (NumWheelsOutOfTrack == NumWheels)
		{
			return true;
		}
	}

	return false;
}

bool UTestOutOfTrackComponent::CheckWheelOutOfTrack(FWheelStatus &WheelStatus) const
{
	if (WheelStatus.bInContact && WheelStatus.PhysMaterial.IsValid())
	{
		FString CurrentPhysMaterialName = WheelStatus.PhysMaterial->GetName();
		for (FString PhysMaterialName : NoTrackPhysicMaterialNames)
		{
			if (CurrentPhysMaterialName == PhysMaterialName)
			{
				return true;
			}
		}
	}
	return false;
}
