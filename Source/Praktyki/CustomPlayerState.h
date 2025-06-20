// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CustomPlayerState.generated.h"

class ARacingGameMode;

/**
 *
 */
UCLASS()
class PRAKTYKI_API ACustomPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void SetupRacingData(int32 NewChecpointCount, ARacingGameMode *GameMode);
	void CheckpointReached(int32 NewCheckpointIndex, bool bFinishLine);
	bool LapFailed();

	UFUNCTION(BlueprintPure)
	int32 GetCompletedLaps() const { return CompletedLaps; }
	UFUNCTION(BlueprintPure)
	float GetCurrentTotalTime();
	UFUNCTION(BlueprintPure)
	float GetCurrentLapTime();
	UFUNCTION(BlueprintPure)
	FString GetCurrentTotalTimeText();
	UFUNCTION(BlueprintPure)
	FString GetCurrentLapTimeText();
	UFUNCTION(BlueprintPure)
	TArray<FString> GetAllLapTimesText();
	UFUNCTION(BlueprintPure)
	int32 GetBestLapIndex() const { return BestLapIndex; }
	UFUNCTION(BlueprintPure)
	FString GetBestLapTimeText();

private:
	ARacingGameMode *RacingGameMode;

	int32 CompletedLaps;

	int32 ChecpointCount;
	int32 LastCheckpoint;
	TArray<int32> CorrectCheckpointsReached;
	TArray<int32> WrongCheckpointsReached;

	float TotalTime;
	TArray<float> LapTimes;
	int32 BestLapIndex;
	bool bLapFailed;

	void UpdateBestLapIndex();

	void LapCompleted();

	bool CheckNextCheckpoint(int32 NewChecpointCount);

	FString FloatTimeToTextTime(float TimeInSeconds);
};
