// Copyright 2025 Teyon. All Rights Reserved.

#include "EngineAudioComponent.h"
#include "PlayerVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UEngineAudioComponent::UEngineAudioComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    PrimaryComponentTick.TickGroup = TG_DuringPhysics;
    bAutoActivate = true;
}

void UEngineAudioComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor *Owner = GetOwner())
    {
        if (APlayerVehiclePawn *PlayerVehiclePawn = Cast<APlayerVehiclePawn>(Owner))
        {
            if (UChaosWheeledVehicleMovementComponent *ChaosWheeledVehicleMovementComponent = PlayerVehiclePawn->GetChaosWheeledVehicleMovementComponent())
            {
                VehicleMovementComponent = ChaosWheeledVehicleMovementComponent;
            }
        }
    }
    SetComponentTickEnabled(true);
}

void UEngineAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateEngineSound();
    TryPlayGearShiftSound();
}

void UEngineAudioComponent::TryPlayGearShiftSound()
{
    if(!VehicleMovementComponent)
    {
        return;
    }

    int32 CurrentGear = VehicleMovementComponent->GetCurrentGear();

    if(CurrentGear != LastGear)
    {
        LastGear = CurrentGear;

        if(CurrentGear == 0) // Neutral
        {
            if (ShiftSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, ShiftSound, GetComponentLocation());
            }
        }
    }
}

void UEngineAudioComponent::UpdateEngineSound()
{
    float Scale;
    if (VehicleMovementComponent)
    {
        float RPM = VehicleMovementComponent->GetEngineRotationSpeed();
        Scale = (RPM - RPMForMinPithAndVolume) / (RPMForMaxPithAndVolume - RPMForMinPithAndVolume);
        Scale = FMath::Clamp(Scale, 0.f, 1.f);
    }
    else
    {
        Scale = 0.5f;
    }

    SetVolumeMultiplier(MinVolume + Scale * MaxExtraVolume);
    SetPitchMultiplier(MinPitch + Scale * MaxExtraPitch);
}
