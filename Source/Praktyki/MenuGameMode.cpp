// Copyright 2025 Teyon. All Rights Reserved.


#include "MenuGameMode.h"
#include "RacingGameInstance.h"
#include "Kismet/GameplayStatics.h"


void AMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    GameInstance = Cast<URacingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstance)
    {
        GameInstance->SelectedCar = AvailablePlayerVehiclePawn[CurrentPlayerVehiclePawnIndex];
    }
}

void AMenuGameMode::ChangeSelectedPlayerVehiclePawn(int32 Diretion)
{
    CurrentPlayerVehiclePawnIndex = (CurrentPlayerVehiclePawnIndex + Diretion + AvailablePlayerVehiclePawn.Num()) % AvailablePlayerVehiclePawn.Num();
    if (GameInstance)
    {
        GameInstance->SelectedCar = AvailablePlayerVehiclePawn[CurrentPlayerVehiclePawnIndex];
    }
}

void AMenuGameMode::ChangeSelectedLapsCount(int32 Diretion)
{
    if (GameInstance)
    {
        GameInstance->SelectedLapsCount = FMath::Clamp(GameInstance->SelectedLapsCount + Diretion, MinLapsCount, MaxLapsCount);
    }
}

void AMenuGameMode::ChangeSelectedMap(int32 Diretion)
{
    if (GameInstance)
    {
        GameInstance->SelectedLapsCount = FMath::Clamp(GameInstance->SelectedLapsCount + Diretion, MinLapsCount, MaxLapsCount);
    }
}

void AMenuGameMode::ChangeSelectedGameMode(int32 Diretion)
{
    if (GameInstance)
    {
        GameInstance->SelectedLapsCount = FMath::Clamp(GameInstance->SelectedLapsCount + Diretion, MinLapsCount, MaxLapsCount);
    }
}
