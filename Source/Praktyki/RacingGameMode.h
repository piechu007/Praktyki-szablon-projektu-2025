// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RacingGameMode.generated.h"

class APlayerVehiclePawn;

/**
 * 
 */
UCLASS()
class PRAKTYKI_API ARacingGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleCheckpointReached(APlayerVehiclePawn *PlayerVehiclePawn, int32 CheckpointIndex, bool bFinishLine);
};
