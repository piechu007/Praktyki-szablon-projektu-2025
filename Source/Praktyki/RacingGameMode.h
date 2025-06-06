// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RacingGameMode.generated.h"

class APlayerVehiclePawn;
class ACustomPlayerState;
class URacingGameInstance;

/**
 * 
 */
UCLASS()
class PRAKTYKI_API ARacingGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void HandlePlayerCompletLap(ACustomPlayerState* PlayerState);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleCheckpointReached(APlayerVehiclePawn *PlayerVehiclePawn, int32 CheckpointIndex, bool bFinishLine);

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();

private:
	URacingGameInstance *GameInstance;
};
