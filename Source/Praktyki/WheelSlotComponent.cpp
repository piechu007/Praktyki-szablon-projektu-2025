// Copyright 2025 Teyon. All Rights Reserved.

#include "WheelSlotComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UWheelSlotComponent::UWheelSlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Mesh"));
}

void UWheelSlotComponent::OnRegister()
{
	Super::OnRegister();

	if (WheelMesh)
	{
		WheelMesh->SetupAttachment(this);
		WheelMesh->SetRelativeLocation(FVector(0.f, 0.f, -SpringMaxDownDistance));
	}
}

// Called when the game starts
void UWheelSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	WheelLength = (WheelRadius * 2.0f * PI); // [cm]
}

// Called every frame
void UWheelSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// RotateWheel(DeltaTime);
}

bool UWheelSlotComponent::Raycast()
{
	float RayDistance = WheelRadius + SpringDefaultDistance;// [cm]

	FVector Start = GetComponentLocation();
	FVector End = Start - (GetUpVector() * RayDistance); 

	FCollisionQueryParams Params;
	Params.bReturnPhysicalMaterial = true;

	return GetWorld()->LineTraceSingleByChannel(CurrentHitResult, Start, End, ECC_GameTraceChannel1, Params, FCollisionResponseParams());
}

void UWheelSlotComponent::UpdateWheelLocationAndVelocity(float DeltaTime)
{
	if (CurrentHitResult.bBlockingHit)
	{
		NewWheelLocation = -GetUpVector() * (CurrentHitResult.Distance - WheelRadius);
	}
	else
	{
		NewWheelLocation = -GetUpVector() * (SpringMaxDownDistance);
	}

	NewWheelWorldLocation = NewWheelLocation + GetComponentLocation();

	WheelVelocity = (NewWheelLocation - LastWheelLocation) / DeltaTime; // [cm/s]
	WheelWorldVelocity = (NewWheelWorldLocation - LastWheelWorldLocation) / DeltaTime; // [cm/s]

	WheelMesh->SetWorldLocation(NewWheelWorldLocation);
	RotateWheel(DeltaTime);

	// ==================  DEBUG  ========================
	// FVector StartDraw = GetComponentLocation();
	// DrawDebugSphere(GetWorld(), CurrentHitResult.Location, 5, 3, FColor::Black, false, 1.f);
	/*if (CurrentDebugUpdateDelay <= 0)
	{
		DrawDebugLine(GetWorld(),
					  CurrentHitResult.Location + (GetForwardVector() * 6.f) + (GetUpVector() * 3.f),
					  CurrentHitResult.Location + (GetForwardVector() * -6.f) + (GetUpVector() * 3.f),
					  bSlipping ? FColor::Red : FColor::Green, false, 1.f, 0, 10.0f);
		CurrentDebugUpdateDelay = DebugUpdateDelay;
	}
	else
	{
		CurrentDebugUpdateDelay--;
	}*/
	// ==================  DEBUG  ========================
	// DrawDebugSphere(GetWorld(), StartDraw + NewWheelLocation, 1, 10, FColor::Red, false, 0.0002f);
	// DrawDebugSphere(GetWorld(), StartDraw + LastWheelLocation, 1, 10, FColor::White, false, 0.0002f);
	// DrawDebugSphere(GetWorld(), StartDraw + NewWheelLocation, WheelRadius, 20, FColor::Red, false, 0.0002f);
}

void UWheelSlotComponent::SaveNewWheelLocationAsLast()
{
	LastWheelLocation = NewWheelLocation;
	LastWheelWorldLocation = NewWheelWorldLocation;
}

void UWheelSlotComponent::RotateWheel(float DeltaTime)
{
	float WheelForwardSpeed = FVector::DotProduct(GetForwardVector(), WheelWorldVelocity); // [cm/s]
	WheelAngularVelocity = 360.f * (WheelForwardSpeed / WheelLength); // [dgr/s]
	float RotateAngle = RotateWheelDirection * WheelAngularVelocity * DeltaTime;
	WheelMesh->AddLocalRotation(FRotator(RotateAngle, 0.f, 0.f));

	// Debug
	//FVector StartDraw = NewWheelWorldLocation + GetRightVector() * 10.f;
	//float Thickenss = 3.f;
	//float LenghtScale = 0.	1f;
	//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (WheelWorldVelocity * LenghtScale), FColor::Blue, false, 0.001f, 9, Thickenss);
	//DrawDebugLine(GetWorld(), StartDraw, StartDraw + (GetForwardVector() * WheelForwardSpeed * LenghtScale), FColor::Orange, false, 0.001f, 10, Thickenss);
	// StartDraw += GetRightVector() * 10.f;
	// DrawDebugLine(GetWorld(), StartDraw, StartDraw + (GetForwardVector() * WheelForwardSpeed * 0.1f), FColor::Cyan, false, 0.001f, 10, 3.0f);
}

void UWheelSlotComponent::SetSlipping(bool bValue)
{
	if (bSlipping == false && bValue == true)
	{
		bSlipping = bValue;
		SlippingStarted();
		return;
	}
	if (bSlipping == true && bValue == false)
	{
		bSlipping = bValue;
		SlippingStoped();
		return;
	}
}

float UWheelSlotComponent::GetFrictionFactor()
{
	float Friction = 2.0f;

	if (CurrentHitResult.PhysMaterial != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentHitResult.PhysMaterial != nullptr"));
		Friction = bSlipping ? KineticFriction : StaticFriction;
		Friction = Friction * CurrentHitResult.PhysMaterial->Friction;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NULLPTR"));
	}

	return Friction;
}
