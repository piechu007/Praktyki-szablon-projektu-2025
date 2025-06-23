// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PlayerVehiclePawn.h"
#include "MenuGameMode.generated.h"

class URacingGameInstance;

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MapName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MapDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D *MapIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MapPath; // np. "/Game/Maps/Mapa1"
};

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
	UFUNCTION(BlueprintCallable)
	void ChangeSelectedLapsCount(int32 Diretion);
	UFUNCTION(BlueprintCallable)
	void ChangeSelectedMap(int32 Diretion);
	UFUNCTION(BlueprintCallable)
	void ChangeSelectedGameMode(int32 Diretion);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMapImage(UTexture2D* MapImage);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateVehicleImage(UTexture2D* VehicleImage);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Race Settings");
	TArray<TSubclassOf<APawn>> AvailablePlayerVehiclePawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Race Settings")
    TArray<UTexture2D*> VehicleImages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Race Settings");
	TArray<FString> MapNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Race Settings")
    TArray<UTexture2D*> MapImages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Race Settings");
	TArray<FString> GameModesNames;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Race Settings");
	int32 MinLapsCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Race Settings");
	int32 MaxLapsCount = 5;

private:
	int32 FindIndexOfLastSelectedCar();
	int32 FindIndexOfLastSelectedMap();
	int32 FindIndexOfLastSelectedGameMode();

	URacingGameInstance *GameInstance;

	int32 CurrentPlayerVehiclePawnIndex = 0;
	int32 CurrentMapIndex = 0;
	int32 CurrentGameModeIndex = 0;
};
