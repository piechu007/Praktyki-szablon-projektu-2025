// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RacingGameMode.h"
#include "TestOutOfTrackComponent.h"
#include "QualifyingGameMode.generated.h"

/**
 *
 */
UCLASS()
class PRAKTYKI_API AQualifyingGameMode : public ARacingGameMode
{
	GENERATED_BODY()

protected:
	virtual void HandleGameStart() override;

	UFUNCTION()
	void HandleVehiclePawnOutOfTrack(AWheeledVehiclePawn *OutOfTrackVehiclePawn);

	UFUNCTION(BlueprintImplementableEvent)
	void LapFailed();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTestOutOfTrackComponent> TestOutOfTrackComponentClass;
};
