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

// Called when the game starts
void UCustomVehicleMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerVehiclePawn = Cast<ACustomPlayerVehiclePawn>(GetOwner());
	PlayerVehiclePawn->GetComponents<UWheelSlotComponent>(WheelSlots);
}

// Called every frame
void UCustomVehicleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateAngleOfSteeringWheels();
	UpdateSpeed();
	AddForcesFromAllWheels(DeltaTime);

	for (UWheelSlotComponent *WheelSlot : WheelSlots)
	{
		WheelSlot->RotateWheel(DeltaTime);
	}
}

void UCustomVehicleMovementComponent::UpdateAngleOfSteeringWheels()
{
	for (UWheelSlotComponent *WheelSlot : WheelSlots)
	{
		if (WheelSlot == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("WheelSlot == nullptr"));
			return;
		}
		if (WheelSlot->bWheelSteering)
		{
			WheelSlot->SetRelativeRotation(FRotator(0.0f, SteeringInput * TurningAngle, 0.f));
		}
	}
}

void UCustomVehicleMovementComponent::SuspensionSpringForce()
{
}

void UCustomVehicleMovementComponent::AddForcesFromAllWheels(float DeltaTime)
{
	for (UWheelSlotComponent *WheelSlot : WheelSlots)
	{
		if (WheelSlot == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("WheelSlot == nullptr"));
			return;
		}
		if (PlayerVehiclePawn == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerVehiclePawn == nullptr"));
			return;
		}
		if (PlayerVehiclePawn->GetMesh() == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerVehiclePawn->GetMesh() == nullptr"));
			return;
		}
		PlayerVehiclePawn->GetMesh()->AddForceAtLocation(GetForceFromWheel(WheelSlot, DeltaTime), WheelSlot->GetComponentLocation());
		//PlayerVehiclePawn->GetMesh()->AddForceAtLocation(GetForceFromWheel(WheelSlot, DeltaTime), WheelSlot->NewWheelWorldLocation);
	}
}

FVector UCustomVehicleMovementComponent::GetForceFromWheel(UWheelSlotComponent *WheelSlot, float DeltaTime)
{
	WheelSlot->Raycast();
	WheelSlot->UpdateWheelLocationAndVelocity(DeltaTime);

	FVector Force = FVector::ZeroVector;

	// ==========  Get Suspertion Force ===============
	float SuspertionForceValue = 0.f;
	if (WheelSlot->CurrentHitResult.bBlockingHit)
	{
		FVector SpringDir = WheelSlot->GetUpVector();
		float SprinOffset = WheelSlot->SpringDefaultDistance + WheelSlot->WheelRadius - WheelSlot->CurrentHitResult.Distance;
		float SprinVelocity = FVector::DotProduct(SpringDir, WheelSlot->WheelVelocity);

		SuspertionForceValue = ((SprinOffset * WheelSlot->SpringStrenght) - (SprinVelocity * WheelSlot->SpringDamper));
		Force += SpringDir * SuspertionForceValue;

		// DEBUG
		FVector StartDraw = WheelSlot->GetComponentLocation();
		DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * ((SprinOffset * WheelSlot->SpringStrenght) - (SprinVelocity * WheelSlot->SpringDamper)) * 0.001f), FColor::Purple, false, 0.0001f, 0, 2.0f); // All Suspertion
		//StartDraw += WheelSlot->GetForwardVector() * 10.f;
		//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * SprinOffset * WheelSlot->SpringStrenght * 0.0002f), FColor::Blue, false, 0.001f, 0, 2.0f); // Spring
		//StartDraw += WheelSlot->GetForwardVector() * 10.f;
		//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * -SprinVelocity * WheelSlot->SpringDamper * 0.0002f), FColor::Red, false, 0.001f, 0, 2.0f); // Damping
		//StartDraw += WheelSlot->GetForwardVector() * 10.f;
		//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * 300000.f * 0.001f), FColor::White, false, 0.0002f, 0, 2.0f); // Gravity
		// UE_LOG(LogTemp, Warning, TEXT("ForceVector.Size() = %f"), ForceVector.Size());
	}

	// ==========  Get Forward Force ===============
	float ForwardForceValue = 0.f;
	if (WheelSlot->CurrentHitResult.bBlockingHit)
	{
		if (BrakeInput > 0.05f && WheelSlot->bWheelBrake)
		{
			ForwardForceValue = -BrakeInput * WheelSlot->BrakeingForce;
		}
		else if (WheelSlot->bWheelDrive)
		{
			if (ThrottleInput > 0.05f)
			{
				ForwardForceValue = ThrottleInput * EngineForce;
			}
			else
			{
				ForwardForceValue = -EngineBrakingForce;
			}
		}

		Force += WheelSlot->GetForwardVector() * ForwardForceValue;
	}

	// ==========  Get Side Force ===============
	float SideForceValue = 0.f;
	if (WheelSlot->CurrentHitResult.bBlockingHit)
	{
		FVector SideDir = WheelSlot->GetRightVector();
		float SideVelocity = FVector::DotProduct(SideDir, WheelSlot->WheelWorldVelocity);

		float accelerationValue = -SideVelocity / DeltaTime;
		SideForceValue = accelerationValue * 50.f;
		Force += WheelSlot->GetRightVector() * SideForceValue;
	}

	WheelSlot->SaveNewWheelLocationAsLast();

	return Force;
}

void UCustomVehicleMovementComponent::UpdateSpeed()
{
	ForwardVelocity = FVector::DotProduct(PlayerVehiclePawn->GetMesh()->GetForwardVector(), PlayerVehiclePawn->GetVelocity());
}


void UCustomVehicleMovementComponent::SetThrottleInput(float Input)
{
	ThrottleInput = Input;
}

void UCustomVehicleMovementComponent::SetSteeringInput(float Input)
{
	SteeringInput = Input;
}

void UCustomVehicleMovementComponent::SetBrakeInput(float Input)
{
	BrakeInput = Input;
}

void UCustomVehicleMovementComponent::SetHandbrakeInput(bool bInput)
{
	HandbrakeInput = bInput;
}
