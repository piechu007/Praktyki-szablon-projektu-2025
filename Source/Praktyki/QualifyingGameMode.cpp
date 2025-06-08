// Copyright 2025 Teyon. All Rights Reserved.

#include "QualifyingGameMode.h"
#include "TestOutOfTrackComponent.h"
#include "CustomPlayerState.h"
#include "PlayerVehiclePawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


void AQualifyingGameMode::HandleGameStart()
{
    Super::HandleGameStart();

    // Add TestOutOfTrackComponet
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        if (APlayerVehiclePawn *PlayerVehiclePawn = Cast<APlayerVehiclePawn>(PlayerController->GetPawnOrSpectator()))
        {
            if (UTestOutOfTrackComponent *TestOutOfTrackComponent = Cast<UTestOutOfTrackComponent>(PlayerVehiclePawn->CreateNewComponet(TestOutOfTrackComponentClass)))
            {
                TestOutOfTrackComponent->OnOutOfTrack.AddDynamic(this, &AQualifyingGameMode::HandleVehiclePawnOutOfTrack);
            }
        }
    }
}

void AQualifyingGameMode::HandleVehiclePawnOutOfTrack(AWheeledVehiclePawn *OutOfTrackVehiclePawn)
{
    AController *Controller = OutOfTrackVehiclePawn->GetController();
    if (Controller)
    {
        ACustomPlayerState *PlayerState = Cast<ACustomPlayerState>(Controller->PlayerState);
        if (PlayerState)
        {
            PlayerState->LapFailed();
            // event ForUI
            LapFailed();
        }
    }
}

