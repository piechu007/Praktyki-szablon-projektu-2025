// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "VehicleSpringArmComponent.generated.h"

class UCameraComponent;

/**
 *
 */
UCLASS()
class PRAKTYKI_API UVehicleSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	void TurnOn(UCameraComponent *CameraComponent);
	void TurnOff();

	void SetLookUpInput(float Value);
	void SetLookRightInput(float Value);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerInput")
	float LeftRightRotationScale = 90.f;
	float UpDownRotationScale = 10.f;
	float BackToDefaultRotationSpeed = 2.f;

private:
	FRotator StartRotation;
};
