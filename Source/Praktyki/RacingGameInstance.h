// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RacingGameInstance.generated.h"

class APawn;

/**
 * 
 */
UCLASS()
class PRAKTYKI_API URacingGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Race Settings")
	TSubclassOf<APawn> SelectedCar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Race Settings")
	int32 SelectedLapsCount;
};
