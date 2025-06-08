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
	virtual void InitGame(const FString &MapName, const FString &Options, FString &ErrorMessage) override;

	void HandlePlayerCompletLap(ACustomPlayerState *PlayerState);

	UFUNCTION(BlueprintPure)
	float GetRacingTime() const;
	UFUNCTION(BlueprintPure)
	float GetStartDelayTime() const;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleCheckpointReached(APlayerVehiclePawn *PlayerVehiclePawn, int32 CheckpointIndex, bool bFinishLine);
	UFUNCTION()
	virtual void HandleGameStart();
	UFUNCTION()
	virtual void HandleRaceStart();

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();
	UFUNCTION(BlueprintImplementableEvent)
	void StartRace();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();
	UFUNCTION(BlueprintImplementableEvent)
	void LapCompleted();

	UPROPERTY(BlueprintReadOnly)
	float StartDelay = 5.f;

private:
	URacingGameInstance *GameInstance;

	bool bIsRaceActive = false;
	float RaceTimer = 0.f;
};
