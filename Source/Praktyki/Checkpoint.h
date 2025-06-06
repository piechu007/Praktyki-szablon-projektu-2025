// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

class UPrimitiveComponent;
class APlayerVehiclePawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCheckpointReached, APlayerVehiclePawn*, PlayerVehiclePawn, int32, CheckpointIndex, bool, bFinishLine);

UCLASS()
class PRAKTYKI_API ACheckpoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckpoint();

	UPROPERTY(BlueprintAssignable)
    FOnCheckpointReached OnCheckpointReached;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(APlayerVehiclePawn *PlayerVehiclePawn, int32 NewCheckpointIndex, bool bReachedFinishLine);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	int32 CheckpointIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	bool bFinishLine = false;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent *BoxCollision;

	UFUNCTION()
	void HandleComponentBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
