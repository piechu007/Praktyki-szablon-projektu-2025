// Copyright 2025 Teyon. All Rights Reserved.


#include "MenuGameMode.h"
#include "RacingGameInstance.h"
#include "Kismet/GameplayStatics.h"


void AMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    GameInstance = Cast<URacingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    CurrentPlayerVehiclePawnIndex = FindIndexOfLastSelectedCar();
    CurrentMapIndex = FindIndexOfLastSelectedMap();
    CurrentGameModeIndex = FindIndexOfLastSelectedGameMode();
}

int32 AMenuGameMode::FindIndexOfLastSelectedCar()
{
    if(GameInstance)
    {
        for (int32 i=0; i<AvailablePlayerVehiclePawn.Num();i++)
        {
            if(AvailablePlayerVehiclePawn[i] == GameInstance->SelectedCar)
            {
                return i;
            }
        }   
    }

    GameInstance->SelectedCar = AvailablePlayerVehiclePawn[0];
    return 0;
}

int32 AMenuGameMode::FindIndexOfLastSelectedMap()
{
    if(GameInstance)
    {
        for (int32 i=0; i<MapNames.Num();i++)
        {
            if(MapNames[i] == GameInstance->SelectedMapName)
            {
                return i;
            }
        }   
    }

    GameInstance->SelectedMapName = MapNames[0];
    return 0;
}

int32 AMenuGameMode::FindIndexOfLastSelectedGameMode()
{
    if(GameInstance)
    {
        for (int32 i=0; i<GameModesNames.Num();i++)
        {
            if(GameModesNames[i] == GameInstance->SelectedGameModeName)
            {
                return i;
            }
        }   
    }
    
    GameInstance->SelectedGameModeName = GameModesNames[0];
    return 0;
}


void AMenuGameMode::ChangeSelectedPlayerVehiclePawn(int32 Diretion)
{
    CurrentPlayerVehiclePawnIndex = (CurrentPlayerVehiclePawnIndex + Diretion + AvailablePlayerVehiclePawn.Num()) % AvailablePlayerVehiclePawn.Num();
    UpdateVehicleImage(VehicleImages[CurrentPlayerVehiclePawnIndex]);
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
    CurrentMapIndex = (CurrentMapIndex + Diretion + MapNames.Num()) % MapNames.Num();
    UpdateMapImage(MapImages[CurrentMapIndex]);
    if (GameInstance)
    {
        GameInstance->SelectedMapName = MapNames[CurrentMapIndex];
    }
}

void AMenuGameMode::ChangeSelectedGameMode(int32 Diretion)
{
    CurrentGameModeIndex = (CurrentGameModeIndex + Diretion + GameModesNames.Num()) % GameModesNames.Num();
    if (GameInstance)
    {
        GameInstance->SelectedGameModeName = GameModesNames[CurrentGameModeIndex];
    }
}
