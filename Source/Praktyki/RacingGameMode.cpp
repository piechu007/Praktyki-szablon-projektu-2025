// Copyright 2025 Teyon. All Rights Reserved.

#include "RacingGameMode.h"
#include "RacingGameInstance.h"
#include "Checkpoint.h"
#include "CustomPlayerState.h"
#include "PlayerVehiclePawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"

void ARacingGameMode::InitGame(const FString &MapName, const FString &Options, FString &ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    // Setup Race Settings from GameInstance
    GameInstance = Cast<URacingGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstance)
    {
        DefaultPawnClass = GameInstance->SelectedCar;
    }
}

void ARacingGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Player Input ModeGameOnly
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        PlayerController->SetInputMode(FInputModeGameOnly());
    }

    // Bind ChceckpointReachEvent
    TArray<AActor *> FoundCheckpoints;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoint::StaticClass(), FoundCheckpoints);
    for (AActor *Actor : FoundCheckpoints)
    {
        ACheckpoint *Checkpoint = Cast<ACheckpoint>(Actor);
        if (Checkpoint)
        {
            Checkpoint->OnCheckpointReached.AddDynamic(this, &ARacingGameMode::HandleCheckpointReached);
        }
    }

    // Reset all Player Racing Data
    if (GameState)
    {
        for (APlayerState *PlayerState : GameState->PlayerArray)
        {
            ACustomPlayerState *CustomPlayerState = Cast<ACustomPlayerState>(PlayerState);
            if (CustomPlayerState)
            {
                CustomPlayerState->SetupRacingData(FoundCheckpoints.Num());
            }
        }
    }
}

void ARacingGameMode::HandlePlayerCompletLap(ACustomPlayerState *PlayerState)
{
    if (PlayerState && PlayerState->GetCompletedLaps() == GameInstance->SelectedLapsCount)
    {
        if (APlayerController *PlayerController = PlayerState->GetPlayerController())
        {
            PlayerController->SetInputMode(FInputModeUIOnly());
            if(APawn* Pawn = PlayerController->GetPawnOrSpectator())
            {
                Pawn->Destroy();
            }
        }

        // TODO: Check All Players
        GameOver();
    }
}

void ARacingGameMode::HandleCheckpointReached(APlayerVehiclePawn *PlayerVehiclePawn, int32 CheckpointIndex, bool bFinishLine)
{
    AController *Controller = PlayerVehiclePawn->GetController();
    if (Controller)
    {
        ACustomPlayerState *PlayerState = Cast<ACustomPlayerState>(Controller->PlayerState);
        if (PlayerState)
        {
            PlayerState->CheckpointReached(CheckpointIndex, bFinishLine);
        }
    }
}
