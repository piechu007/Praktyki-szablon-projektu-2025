// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "PlayerVehiclePawn.generated.h"

class UInputAction;
class UChaosWheeledVehicleMovementComponent;
class UActorComponent;
class UVehicleSpringArmComponent;
class UVehicleCameraComponent;
class UEngineAudioComponent;

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

	void SetLockHandbrakeState(bool bNewLockHandbake);

	UFUNCTION(BlueprintPure)
	UChaosWheeledVehicleMovementComponent *GetChaosWheeledVehicleMovementComponent() const;

	UActorComponent *CreateNewComponet(TSubclassOf<UActorComponent> ComponentClass);

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext *VehicleMapingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *SteeringAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *ThrottleAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *BrakeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *HandbrakeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *ToggleCameraAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *LookUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *LookRightAction;

	void SetSteeringInput(const FInputActionValue &Value);
	void SetThrottleInput(const FInputActionValue &Value);
	void SetBrakeInput(const FInputActionValue &Value);
	void SetHandbrakeTriggeredInput(const FInputActionValue &Value);
	void SetHandbrakeCompletedInput(const FInputActionValue &Value);
	void SetToggleCameraInput(const FInputActionValue &Value);
	void SetLookUpInput(const FInputActionValue &Value);
	void SetLookRightInput(const FInputActionValue &Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	int32 CameraBoomsCount = 3;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TArray<UVehicleSpringArmComponent *> CameraBooms;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UVehicleCameraComponent *FollowCamera;
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UEngineAudioComponent *EngineSound;

private:
	int32 CurrentCameraBoomIndex;

	bool bLockHandbreak = true;

	class APlayerController *VehiclePlayerController;
};
