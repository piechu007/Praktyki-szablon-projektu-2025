// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PlayerVehiclePawn.h"
#include "MenuGameMode.generated.h"

class URacingGameInstance;

/**
 * 
 */
UCLASS()
class PRAKTYKI_API AMenuGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ChangeSelectedPlayerVehiclePawn(int32 Diretion);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Race Settings");
	TArray<TSubclassOf<APlayerVehiclePawn>> AvailablePlayerVehiclePawn;

private:
	int32 CurrentPlayerVehiclePawnIndex = 0;

	URacingGameInstance *GameInstance;
};
