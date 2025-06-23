// Copyright 2025 Teyon. All Rights Reserved.


#include "CustomVehicleMovementComponent.h"
#include "WheelSlotComponent.h"
#include "CustomPlayerVehiclePawn.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UCustomVehicleMovementComponent::UCustomVehicleMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCustomVehicleMovementComponent::Setup(ACustomPlayerVehiclePawn* CustomPlayerVehiclePawn, TArray<UWheelSlotComponent*> WheelSlotComponts)
{
	WheelSlots = WheelSlotComponts;
	PlayerVehiclePawn = CustomPlayerVehiclePawn;
}


// Called when the game starts
void UCustomVehicleMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCustomVehicleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for(UWheelSlotComponent* WheelSlot: WheelSlots)
	{
		if(WheelSlot == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("WheelSlot == nullptr"));
			return;
		}
		if(PlayerVehiclePawn == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerVehiclePawn == nullptr"));
			return;
		}
		if(PlayerVehiclePawn->GetMesh() == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerVehiclePawn->GetMesh() == nullptr"));
			return;
		}
		PlayerVehiclePawn->GetMesh()->AddForceAtLocation(WheelSlot->GetForce(DeltaTime), WheelSlot->GetComponentLocation());
	}
	//PlayerVehiclePawn->GetMesh()->AddForce(1200000.f * PlayerVehiclePawn->GetActorUpVector()); 
}

void UCustomVehicleMovementComponent::SuspensionSpringForce()
{
	
}
 
