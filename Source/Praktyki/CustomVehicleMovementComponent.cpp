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
	AirDragForceOffset = FVector(0.f, 0.f, 0.f);
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
	AddAirDragForce();
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
		PlayerVehiclePawn->GetMesh()->AddForceAtLocation(GetForceFromWheel(WheelSlot, DeltaTime), WheelSlot->NewWheelWorldLocation); // [cN]
	}
}

FVector UCustomVehicleMovementComponent::GetForceFromWheel(UWheelSlotComponent *WheelSlot, float DeltaTime)
{
	WheelSlot->Raycast();
	WheelSlot->UpdateWheelLocationAndVelocity(DeltaTime);	//
	WheelSlot->SaveNewWheelLocationAsLast();

	FVector Force = FVector::ZeroVector;

	if (!WheelSlot->CurrentHitResult.bBlockingHit)
	{
		return Force;
	}

	FVector FrictionForce = FVector::ZeroVector;

	// ==========  Get Suspertion Force ===============
	float SuspertionForceValue = 0.f;
	FVector SpringDir = WheelSlot->GetUpVector();
	float SprinOffset = (WheelSlot->SpringDefaultDistance + WheelSlot->WheelRadius - WheelSlot->CurrentHitResult.Distance); // [cm] 
	float SprinVelocity = FVector::DotProduct(SpringDir, WheelSlot->WheelVelocity); // [cm/s] 

	SuspertionForceValue = ((SprinOffset * WheelSlot->SpringStrenght) - (SprinVelocity * WheelSlot->SpringDamper)); // [cN]
	Force += SpringDir * SuspertionForceValue;

	// ==========  Get Forward Force ===============
	float ForwardForceValue = 0.f;
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
	FrictionForce += WheelSlot->GetForwardVector() * ForwardForceValue;

	// ==========  Get Side Force ===============
	float SideForceValue = 0.f;
	FVector SideDir = WheelSlot->GetRightVector();
	float SideVelocity = FVector::DotProduct(SideDir, WheelSlot->WheelWorldVelocity);
	float accelerationValue = -SideVelocity / DeltaTime;

	SideForceValue = accelerationValue * WheelSlot->SideForceFactor;
	FrictionForce += WheelSlot->GetRightVector() * SideForceValue;

	// ========== Testing Slipping ===============
	float FrictionForceMaxValue = FMath::Abs(SuspertionForceValue * WheelSlot->GetFrictionFactor());
	float FrictionForceCurrentValueSquared = SideForceValue * SideForceValue + ForwardForceValue * ForwardForceValue;
	if (FrictionForceCurrentValueSquared > FrictionForceMaxValue * FrictionForceMaxValue)
	{
		WheelSlot->SetSlipping(true);
		FrictionForce = FrictionForce.GetSafeNormal() * FrictionForceMaxValue;
	}
	else
	{
		WheelSlot->SetSlipping(false);
	}
	Force += FrictionForce;

	// DEBUG
	//FVector StartDraw = WheelSlot->NewWheelWorldLocation;
	//float Thickenss = 3.f;
	//float LenghtScale = 0.0001f;
	//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (WheelSlot->GetForwardVector() * (ForwardForceValue)*LenghtScale), FColor::White, false, 0.0001f, 2, Thickenss);
	//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (WheelSlot->GetRightVector() * (SideForceValue)*LenghtScale), FColor::Yellow, false, 0.0001f, 2, Thickenss);
	//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (WheelSlot->GetUpVector() * (SuspertionForceValue)*LenghtScale), FColor::Blue, false, 0.0001f, 2, Thickenss);
	//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (Force)*LenghtScale, FColor::White, false, 0.0001f, 1, Thickenss);
	//StartDraw -= WheelSlot->GetUpVector() * 10.f;
	//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (FrictionForce.GetSafeNormal() * FrictionForceMaxValue) * LenghtScale, FColor::Magenta, false, 0.0001f, 3, Thickenss);
	//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (FrictionForce)*LenghtScale, WheelSlot->bSlipping ? FColor::Red : FColor::Green, false, 0.0001f, 4, Thickenss);

	return Force;
}

void UCustomVehicleMovementComponent::AddDownforce()
{
	float DownforceValue = ForwardVelocity * ForwardVelocity * DownforceFactor;

	FVector DownforceLocation = PlayerVehiclePawn->GetMesh()->GetComponentTransform().TransformPosition(DownforceOffset);
	// PlayerVehiclePawn->GetMesh()->AddForceAtLocation(PlayerVehiclePawn->GetMesh()->GetUpVector() * -DownforceValue, DownforceLocation);
	PlayerVehiclePawn->GetMesh()->AddForce(PlayerVehiclePawn->GetMesh()->GetUpVector() * -DownforceValue);

	// DEBUG
	//float Thickenss = 3.f;
	//float LenghtScale = 0.0002f;
	//DrawDebugLine(GetWorld(), DownforceLocation, DownforceLocation + (PlayerVehiclePawn->GetMesh()->GetUpVector() * (-DownforceValue) * LenghtScale), FColor::Cyan, false, 0.0001f, 1, Thickenss);
}

void UCustomVehicleMovementComponent::AddAirDragForce()
{
	float AirDragForceValue = ForwardVelocity * ForwardVelocity * AirDragForceFactor;

	FVector AirDragForceLocation = PlayerVehiclePawn->GetMesh()->GetComponentTransform().TransformPosition(AirDragForceOffset);
	PlayerVehiclePawn->GetMesh()->AddForce(PlayerVehiclePawn->GetMesh()->GetForwardVector() * -AirDragForceValue);

	// DEBUG
	//float Thickenss = 3.f;
	//float LenghtScale = 0.0002f;
	//DrawDebugLine(GetWorld(), AirDragForceLocation, AirDragForceLocation + (PlayerVehiclePawn->GetMesh()->GetForwardVector() * (-AirDragForceValue) * LenghtScale), FColor::Orange, false, 0.0001f, 1, Thickenss);
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

	TryAutomaticlyChangeGear();
	CurrentRPM = FMath::Clamp(DriveWheelsAverageAngularVelocity * CurrentGearRatio, IdleRPM, MaxRPM);

	CurrentEngineTorque = ThrottleCurve->GetFloatValue(1.f - (MaxRPM - CurrentRPM) / MaxRPM) * EngineMaxTorque * 100000.f / DriveWheelsNum; // *100 cm->m *1000 N
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

float UCustomVehicleMovementComponent::GetCurrentRPM() const
{
	return CurrentRPM;
}