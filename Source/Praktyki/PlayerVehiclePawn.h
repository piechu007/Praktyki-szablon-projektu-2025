// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "PlayerVehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class PRAKTYKI_API APlayerVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	APlayerVehiclePawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//virtual void BeginPlay() override;

	//void MoveForward(float Value);
	//void MoveRight(float Value);
	//void Turn(float Value);
	//void LookUp(float Value);

	void SetThrottleInput(float Value);
	void SetBrakeInput(float Value);
	void SetSteeringInput(float Value);

	void SetLookUpInput(float Value);
	void SetLookRihtInput(float Value);

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;
};

