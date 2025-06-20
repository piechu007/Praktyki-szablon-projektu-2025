// Copyright 2025 Teyon. All Rights Reserved.

#include "CustomPlayerState.h"
#include "RacingGameMode.h"
#include "Engine/World.h"

void ACustomPlayerState::SetupRacingData(int32 NewChecpointCount, ARacingGameMode *GameMode)
{
    RacingGameMode = GameMode;
    CompletedLaps = 0;
    ChecpointCount = NewChecpointCount;
    LastCheckpoint = 0;
    BestLapIndex = 0;
    CorrectCheckpointsReached.Empty();
    CorrectCheckpointsReached.Add(0);
    WrongCheckpointsReached.Empty();
    TotalTime = 0;
    LapTimes.Empty();
}

float ACustomPlayerState::GetCurrentTotalTime()
{
    if (RacingGameMode)
    {
        return RacingGameMode->GetRacingTime();
    }
    return 0.f;
}

float ACustomPlayerState::GetCurrentLapTime()
{
    if (RacingGameMode)
    {
        return RacingGameMode->GetRacingTime() - TotalTime;
    }
    return 0.f;
}

FString ACustomPlayerState::GetCurrentTotalTimeText()
{
    return FloatTimeToTextTime(GetCurrentTotalTime());
}

FString ACustomPlayerState::GetCurrentLapTimeText()
{
    if(bLapFailed)
    {
        return FloatTimeToTextTime(-1.f);
    }
    return FloatTimeToTextTime(GetCurrentLapTime());
}

TArray<FString> ACustomPlayerState::GetAllLapTimesText()
{
    TArray<FString> AllLapTimesText;
    for (float LapTimeText : LapTimes)
    {
        AllLapTimesText.Add(FloatTimeToTextTime(LapTimeText));
    }
    return AllLapTimesText;
}

FString ACustomPlayerState::GetBestLapTimeText()
{
    if(CompletedLaps == 0)
    {
        return FloatTimeToTextTime(0.f);
    }
    return FloatTimeToTextTime(LapTimes[BestLapIndex]);
}

void ACustomPlayerState::UpdateBestLapIndex()
{
    if (LapTimes.Num() == 0)
    {
        return;
    }

    int32 MinIndex = 0;
    float MinTime = LapTimes[0];

    for (int32 i = 1; i < LapTimes.Num(); i++)
    {
        if ((LapTimes[i] < MinTime && LapTimes[i] > 0.f) || MinTime < -0.1f)// BeterLapTime or MinIndex Lap Was Failed
        {
            MinTime = LapTimes[i];
            MinIndex = i;
        }
    }

    BestLapIndex = MinIndex;
}

void ACustomPlayerState::CheckpointReached(int32 NewCheckpointIndex, bool bFinishLine)
{
    if (NewCheckpointIndex == LastCheckpoint)
    {
        return;
    }

    if (CheckNextCheckpoint(NewCheckpointIndex)) // Drive Correct Way
    {
        if (WrongCheckpointsReached.Num() == 0)
        {
            // Increase Ceorrect Way
            if (CorrectCheckpointsReached.Num() == ChecpointCount)
            {
                // Reset Ceorrect Way For New Lap
                CorrectCheckpointsReached.Empty();
                LapCompleted();
            }
            CorrectCheckpointsReached.Add(NewCheckpointIndex);
        }
        else
        {
            // Decrese Wrong Way
            WrongCheckpointsReached.Pop();
        }
    }
    else // Drive Wrong Way
    {
        if (CorrectCheckpointsReached.Num() == 0)
        {
            // Increase Wrong Way
            WrongCheckpointsReached.Add(NewCheckpointIndex);
        }
        else
        {
            // Decrese Ceorrect Way
            CorrectCheckpointsReached.Pop();
        }
    }
    LastCheckpoint = NewCheckpointIndex;

    /*FString log = TEXT("CheckpointReached ") + FString::FromInt(NewCheckpointIndex);
    log += TEXT("\n CorrectCheckpointsReached:  ");
    for (int32 index : CorrectCheckpointsReached)
        log += FString::FromInt(index) + TEXT(", ");
    log += TEXT("\n WrongCheckpointsReached:    ");
    for (int32 index : WrongCheckpointsReached)
        log += FString::FromInt(index) + TEXT(", ");
    UE_LOG(LogTemp, Warning, TEXT("%s"), *log);*/
}

bool ACustomPlayerState::CheckNextCheckpoint(int32 NewCheckpointIndex)
{
    if ((NewCheckpointIndex - 1 == LastCheckpoint) || (NewCheckpointIndex == 0 && LastCheckpoint == ChecpointCount - 1))
    {
        return true;
    }
    return false;
}

bool ACustomPlayerState::LapFailed()
{
    if(!bLapFailed)
    {
        bLapFailed = true;
        return true;
    }
    return false;
}

void ACustomPlayerState::LapCompleted()
{
    CompletedLaps++;
    if(bLapFailed)
    {
        LapTimes.Add(-1.f);
    }
    else
    {
        LapTimes.Add(RacingGameMode->GetRacingTime() - TotalTime);
    }

    TotalTime = RacingGameMode->GetRacingTime();
    UpdateBestLapIndex();
    bLapFailed = false;

    if (RacingGameMode)
    {
        RacingGameMode->HandlePlayerCompletLap(this);
    }
}

FString ACustomPlayerState::FloatTimeToTextTime(float TimeInSeconds)
{
    if(TimeInSeconds < -0.1f)
    {
        return "Failed";
    }

    int32 TotalSeconds = FMath::FloorToInt(TimeInSeconds);
    int32 Minutes = TotalSeconds / 60;
    int32 Seconds = TotalSeconds % 60;

    int32 Hundredths = FMath::RoundToInt((TimeInSeconds - TotalSeconds) * 100);

    return FString::Printf(TEXT("%02d:%02d.%02d"), Minutes, Seconds, Hundredths);
}
