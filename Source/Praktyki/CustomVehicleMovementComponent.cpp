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

	ForwardGearsRatio = {15.f, 10.f, 7.f, 5.f, 3.f};
	BackGearsRatio = {-14.f};
	DownforceOffset = FVector(-50.f, 0.f, 0.f);
}

// Called when the game starts
void UCustomVehicleMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerVehiclePawn = Cast<ACustomPlayerVehiclePawn>(GetOwner());
	PlayerVehiclePawn->GetComponents<UWheelSlotComponent>(WheelSlots);

	UE_LOG(LogTemp, Warning, TEXT("UCustomVehicleMovementComponent::BeginPlay()"));

	CurrentGear = 1;
	CurrentGearRatio = ForwardGearsRatio[CurrentGear - 1];
}

// Called every frame
void UCustomVehicleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateAngleOfSteeringWheels();
	UpdateSpeed();
	UpdateEngine();

	AddForcesFromAllWheels(DeltaTime);
	AddDownforce();
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
			if (SteeringCurve)
			{
				WheelSlot->SetRelativeRotation(FRotator(0.0f, SteeringInput * SteeringCurve->GetFloatValue(ForwardVelocity), 0.f));
			}
			else
			{
				WheelSlot->SetRelativeRotation(FRotator(0.0f, SteeringInput * DefaultSteeringAngle, 0.f));
			}
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
		PlayerVehiclePawn->GetMesh()->AddForceAtLocation(GetForceFromWheel(WheelSlot, DeltaTime), WheelSlot->NewWheelWorldLocation);
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

		// DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * SprinOffset * WheelSlot->SpringStrenght * 0.0002f), FColor::Blue, false, 0.001f, 0, 2.0f); // Spring
		// StartDraw += WheelSlot->GetForwardVector() * 10.f;
		// DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * -SprinVelocity * WheelSlot->SpringDamper * 0.0002f), FColor::Red, false, 0.001f, 0, 2.0f); // Damping
		// StartDraw += WheelSlot->GetForwardVector() * 10.f;
		// DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * 300000.f * 0.001f), FColor::White, false, 0.0002f, 0, 2.0f); // Gravity
		// UE_LOG(LogTemp, Warning, TEXT("SuspertionForceValue = %f"), SuspertionForceValue);
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
				ForwardForceValue = ThrottleInput * (CurrentEngineTorque / WheelSlot->WheelRadius);
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
		// SideForceValue = accelerationValue * SuspertionForceValue * 0.001f;
		SideForceValue = accelerationValue * SteeringForceFactor;
		Force += WheelSlot->GetRightVector() * SideForceValue;
	}

	// DEBUG
	FVector StartDraw = WheelSlot->GetComponentLocation();
	float Thickenss = 3.f;
	float LenghtScale = 0.001f;
	DrawDebugLine(GetWorld(), StartDraw, StartDraw + (WheelSlot->GetForwardVector() * (ForwardForceValue)*LenghtScale), FColor::Red, false, 0.0001f, 1, Thickenss);
	DrawDebugLine(GetWorld(), StartDraw, StartDraw + (WheelSlot->GetRightVector() * (SideForceValue)*LenghtScale), FColor::Green, false, 0.0001f, 2, Thickenss);
	DrawDebugLine(GetWorld(), StartDraw, StartDraw + (WheelSlot->GetUpVector() * (SuspertionForceValue)*LenghtScale), FColor::Blue, false, 0.0001f, 3, Thickenss);
	DrawDebugLine(GetWorld(), StartDraw, StartDraw + (WheelSlot->GetUpVector() * SuspertionForceValue + WheelSlot->GetRightVector() * SideForceValue + WheelSlot->GetForwardVector() * ForwardForceValue) * LenghtScale, FColor::White, false, 0.0001f, 0, Thickenss);

	WheelSlot->SaveNewWheelLocationAsLast();

	return Force;
}

void UCustomVehicleMovementComponent::AddDownforce()
{
	float DownforceValue = ForwardVelocity * ForwardVelocity * DownforceFactor;

	FVector DownForceLocation = PlayerVehiclePawn->GetMesh()->GetComponentTransform().TransformPosition(DownforceOffset);
	PlayerVehiclePawn->GetMesh()->AddForceAtLocation(PlayerVehiclePawn->GetMesh()->GetUpVector() * -DownforceValue, DownForceLocation);

	// DEBUG
	float Thickenss = 3.f;
	float LenghtScale = 0.001f;
	DrawDebugLine(GetWorld(), DownForceLocation, DownForceLocation + (PlayerVehiclePawn->GetMesh()->GetUpVector() * (-DownforceValue) * LenghtScale), FColor::Orange, false, 0.0001f, 1, Thickenss);
}

void UCustomVehicleMovementComponent::UpdateSpeed()
{
	ForwardVelocity = FVector::DotProduct(PlayerVehiclePawn->GetMesh()->GetForwardVector(), PlayerVehiclePawn->GetVelocity()) * 0.036f;
}

void UCustomVehicleMovementComponent::UpdateEngine()
{
	float DriveWheelsAverageAngularVelocity = 0.f;
	int32 DriveWheelsNum = 0;
	for (UWheelSlotComponent *WheelSlot : WheelSlots)
	{
		if (WheelSlot->bWheelDrive)
		{
			DriveWheelsAverageAngularVelocity += WheelSlot->WheelAngularVelocity;
			DriveWheelsNum++;
		}
	}
	DriveWheelsAverageAngularVelocity = FMath::Abs(DriveWheelsAverageAngularVelocity / DriveWheelsNum);
	// UE_LOG(LogTemp, Display, TEXT("DriveWheelsAverageAngularVelocity = %f"), DriveWheelsAverageAngularVelocity);

	TryAutomaticlyChangeGear();
	CurrentRPM = FMath::Clamp(DriveWheelsAverageAngularVelocity * CurrentGearRatio, IdleRPM, MaxRPM);
	// UE_LOG(LogTemp, Display, TEXT("CurrentRPM                        = %f"), CurrentRPM);

	CurrentEngineTorque = ThrottleCurve->GetFloatValue(1.f - (MaxRPM - CurrentRPM) / MaxRPM) * EngineMaxTorque * 100000.f / DriveWheelsNum; // *100 cm->m *1000 N
	// UE_LOG(LogTemp, Display, TEXT("CurrentEngineTorque               = %f"), CurrentEngineTorque);
}

void UCustomVehicleMovementComponent::TryAutomaticlyChangeGear()
{
	if (CurrentRPM > ChangeUpRPM)
	{
		ChangeGear(1);
		return;
	}
	if (CurrentRPM < ChangeDownRPM)
	{
		ChangeGear(-1);
		return;
	}
}

void UCustomVehicleMovementComponent::ChangeGear(int32 dir)
{
	int32 TargetGear = CurrentGear + dir;
	if (TargetGear > 0)
	{
		if (TargetGear >= ForwardGearsRatio.Num())
		{
			TargetGear = ForwardGearsRatio.Num();
		}
		CurrentGear = TargetGear;
		CurrentGearRatio = ForwardGearsRatio[CurrentGear - 1];
	}
	else if (TargetGear < 0)
	{
		if (TargetGear <= -BackGearsRatio.Num())
		{
			TargetGear = -BackGearsRatio.Num();
		}
		CurrentGear = TargetGear;
		CurrentGearRatio = BackGearsRatio[-CurrentGear - 1];
	}
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

float UCustomVehicleMovementComponent::GetForwardVelocity() const
{
	return ForwardVelocity;
}

int32 UCustomVehicleMovementComponent::GetCurrentGear() const
{
	return CurrentGear;
}

float UCustomVehicleMovementComponent::GetCurrentCurrentRPM() const
{
	return CurrentRPM;
}