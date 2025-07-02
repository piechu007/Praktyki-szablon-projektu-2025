// Copyright 2025 Teyon. All Rights Reserved.

#include "EngineAudioComponent.h"
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
    int32 CurrentGear = GetCurrentGear();

    if(CurrentGear != LastGear)
    {
        UE_LOG(LogTemp, Warning, TEXT("LastGear = %d,  CurrentGear = %d"), LastGear, CurrentGear);

        LastGear = CurrentGear;

        if(CurrentGear != 0) // No Neutral
        {
            if (ShiftSound)
            {
                UE_LOG(LogTemp, Warning, TEXT("PlayShiftSound"));
                UGameplayStatics::PlaySoundAtLocation(this, ShiftSound, GetComponentLocation());
            }
        }
    }
}

void UEngineAudioComponent::UpdateEngineSound()
{
    float Scale = (GetCurrentRPM() - RPMForMinPithAndVolume) / (RPMForMaxPithAndVolume - RPMForMinPithAndVolume);
    Scale = FMath::Clamp(Scale, 0.f, 1.f);

    SetVolumeMultiplier(MinVolume + Scale * MaxExtraVolume);
    SetPitchMultiplier(MinPitch + Scale * MaxExtraPitch);
}

int32 UEngineAudioComponent::GetCurrentGear()
{
    return 0;
}

float UEngineAudioComponent::GetCurrentRPM()
{
    return 0.0f;
}