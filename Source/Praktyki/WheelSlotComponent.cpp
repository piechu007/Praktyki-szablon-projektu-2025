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
}

// Called every frame
void UWheelSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector UWheelSlotComponent::GetForce(float DeltaTime)
{
	FVector Force = FVector::ZeroVector;

	FHitResult HitResult;
	if (Raycast())
	{
		UpdateWheelLocationAndVelocity(DeltaTime);
		Force += GetSuspertionForce(DeltaTime);
		Force += GetSideForce(DeltaTime);
		Force += GetForwardForce(DeltaTime);
	}
	else
	{
		UpdateWheelLocationAndVelocity(DeltaTime);
	}

	LastWheelLocation = NewWheelLocation;

	return Force;
}

bool UWheelSlotComponent::Raycast()
{
	float RayDistance = WheelRadius + SpringDefaultDistance;

	FVector Start = GetComponentLocation();
	FVector End = Start - (GetUpVector() * RayDistance);
	
	// ==================  DEBUG  ========================
	//FVector StartDraw = GetComponentLocation();
	//DrawDebugSphere(GetWorld(), StartDraw + NewWheelLocation, 1, 10, FColor::Red, false, 0.0002f);

	return GetWorld()->LineTraceSingleByChannel(CurrentHitResult, Start, End, ECC_GameTraceChannel1, FCollisionQueryParams(), FCollisionResponseParams());;
}

void UWheelSlotComponent::UpdateWheelLocationAndVelocity(float DeltaTime)
{
	if (CurrentHitResult.bBlockingHit)
	{
		NewWheelLocation = - GetUpVector() * (CurrentHitResult.Distance - WheelRadius);
		DrawDebugLine(GetWorld(), GetComponentLocation(), GetComponentLocation() + (GetUpVector() * 100.f), FColor::Yellow, false, 0.0001f, 0, 2.0f); // 0.1 time 2.0 thickness
	}
	else
	{
		NewWheelLocation = - GetUpVector() * (SpringMaxDownDistance);
		DrawDebugLine(GetWorld(), GetComponentLocation(), GetComponentLocation() + (GetUpVector() * 100.f), FColor::Green, false, 0.0001f, 0, 2.0f); // 0.1 time 2.0 thickness
	}

	NewWheelWorldLocation = NewWheelLocation + GetComponentLocation();

	WheelVelocity = (NewWheelLocation - LastWheelLocation) / DeltaTime;
	WheelWorldVelocity = (NewWheelWorldLocation - LastWheelWorldLocation) / DeltaTime;

	WheelMesh->SetWorldLocation(NewWheelWorldLocation);

	// ==================  DEBUG  ========================
	//FVector StartDraw = GetComponentLocation();
	DrawDebugSphere(GetWorld(), CurrentHitResult.Location, 5, 3, FColor::Black, false, 1.f);
	//DrawDebugSphere(GetWorld(), StartDraw + NewWheelLocation, 1, 10, FColor::Red, false, 0.0002f);
	//DrawDebugSphere(GetWorld(), StartDraw + LastWheelLocation, 1, 10, FColor::White, false, 0.0002f);
	//DrawDebugSphere(GetWorld(), StartDraw + NewWheelLocation, WheelRadius, 20, FColor::Red, false, 0.0002f);
}

void UWheelSlotComponent::SaveNewWheelLocationAsLast()
{
	LastWheelLocation = NewWheelLocation;
	LastWheelWorldLocation = NewWheelWorldLocation;
}

void UWheelSlotComponent::RotateWheel(float DeltaTime)
{
	float WheelForwardSpeed = FVector::DotProduct(GetForwardVector(), WheelWorldVelocity);
	float RotateAngle = RotateWheelDirection * 360.f * (WheelForwardSpeed / (WheelRadius * 2 * PI)) * DeltaTime;
	WheelMesh->AddLocalRotation(FRotator(RotateAngle, 0.f, 0.f));
}


FVector UWheelSlotComponent::GetSuspertionForce(float DeltaTime)
{
	FVector SpringDir = GetUpVector();

	float SprinOffset = SpringDefaultDistance + WheelRadius - CurrentHitResult.Distance;

	float SprinVelocity = FVector::DotProduct(SpringDir, WheelVelocity);

	FVector ForceVector = SpringDir * ((SprinOffset * SpringStrenght) - (SprinVelocity * SpringDamper));

	// DEBUG
	FVector StartDraw = GetComponentLocation();
	DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * ((SprinOffset * SpringStrenght) - (SprinVelocity * SpringDamper)) * 0.0002f), FColor::Purple, false, 0.0001f, 0, 2.0f); // 0.1 time 2.0 thickness
	StartDraw += GetForwardVector() * 10.f;
	DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * SprinOffset * SpringStrenght * 0.0002f), FColor::Blue, false, 0.001f, 0, 2.0f); // Spring
	StartDraw += GetForwardVector() * 10.f;
	DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * -SprinVelocity * SpringDamper * 0.0002f), FColor::Red, false, 0.001f, 0, 2.0f); // Damping
	StartDraw += GetForwardVector() * 10.f;
	DrawDebugLine(GetWorld(), StartDraw, StartDraw + (SpringDir * 300000.f * 0.001f), FColor::White, false, 0.0002f, 0, 2.0f); // Gravity
	UE_LOG(LogTemp, Warning, TEXT("ForceVector.Size() = %f"), ForceVector.Size());

	return ForceVector;
}

FVector UWheelSlotComponent::GetSideForce(float DeltaTime)
{
	FVector Force = FVector::ZeroVector;

	return Force;
}

FVector UWheelSlotComponent::GetForwardForce(float DeltaTime)
{
	FVector Force = FVector::ZeroVector;

	return Force;
}
