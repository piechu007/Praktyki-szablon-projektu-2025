// Copyright 2025 Teyon. All Rights Reserved.

#include "WheelSlotComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UWheelSlotComponent::UWheelSlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UWheelSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
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
	if (Raycast(HitResult))
	{
		UpdateWheelLocationAndVelocity(HitResult, DeltaTime);
		Force += GetSuspertionForce(HitResult, DeltaTime);
		Force += GetSideForce(HitResult, DeltaTime);
		Force += GetForwardForce(HitResult, DeltaTime);
	}
	else
	{
		UpdateWheelLocationAndVelocity(HitResult, DeltaTime);
	}

	LastWheelLocation = NewWheelLocation;

	return Force;
}

bool UWheelSlotComponent::Raycast(FHitResult &OutHit)
{
	float RayDistance = WheelRadius + SpringMaxDownDistance;

	FVector Start = GetComponentLocation();
	FVector End = Start - (GetUpVector() * RayDistance);

	bool HasHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_GameTraceChannel1, FCollisionQueryParams(), FCollisionResponseParams());

	if (HasHit)
	{
		// FString ComponetName = GetName();
		// FString HitActorName = OutHit.GetActor()->GetActorNameOrLabel();
		// float Time = GetWorld()->TimeSeconds;
		// UE_LOG(LogTemp, Warning, TEXT("%s    hit         < %s >    [ %f ]"), *ComponetName, *HitActorName, Time);
		// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.0001f, 0, 2.0f); // 0.1 time 2.0 thickness
	}
	else
	{
		// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.0001f, 0, 2.0f); // 0.1 time 2.0 thickness
	}

	return HasHit;
}

void UWheelSlotComponent::UpdateWheelLocationAndVelocity(FHitResult &HitResult, float DeltaTime)
{
	if (HitResult.bBlockingHit)
	{
		//NewWheelLocation = GetComponentLocation() - GetUpVector() * (HitResult.Distance - WheelRadius);
		NewWheelLocation = - GetUpVector() * (HitResult.Distance - WheelRadius);
	}
	else
	{
		//NewWheelLocation = GetComponentLocation() - GetUpVector() * (SpringMaxDownDistance);
		NewWheelLocation = - GetUpVector() * (SpringMaxDownDistance);
	}
	WheelVelocity = (NewWheelLocation - LastWheelLocation) / DeltaTime;

	// DEBUG
	FVector StartDraw = GetComponentLocation();
	DrawDebugSphere(GetWorld(), StartDraw + NewWheelLocation, 1, 10, FColor::Red, false, 0.0002f);
	DrawDebugSphere(GetWorld(), StartDraw + LastWheelLocation, 1, 10, FColor::White, false, 0.0002f);
	DrawDebugSphere(GetWorld(), StartDraw + NewWheelLocation, WheelRadius, 20, FColor::Red, false, 0.0002f);
}

FVector UWheelSlotComponent::GetSuspertionForce(FHitResult &HitResult, float DeltaTime)
{
	FVector SpringDir = GetUpVector();

	float SprinOffset = SpringDefaultDistance + WheelRadius - HitResult.Distance;

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

FVector UWheelSlotComponent::GetSideForce(FHitResult &HitResult, float DeltaTime)
{
	FVector Force = FVector::ZeroVector;

	return Force;
}

FVector UWheelSlotComponent::GetForwardForce(FHitResult &HitResult, float DeltaTime)
{
	FVector Force = FVector::ZeroVector;

	return Force;
}
