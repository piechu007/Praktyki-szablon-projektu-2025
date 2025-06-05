// Copyright 2025 Teyon. All Rights Reserved.

#include "RacingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "RacingGameInstance.h"
#include "GameFramework/PlayerController.h"

void ARacingGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{    
    Super::InitGame(MapName, Options, ErrorMessage);

    URacingGameInstance *GameInstance = Cast<URacingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstance)
    {
        DefaultPawnClass = GameInstance->SelectedCar;
    }
}

void ARacingGameMode::BeginPlay()
{
    Super::BeginPlay();

    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        PlayerController->SetInputMode(FInputModeGameOnly());
    }
}
