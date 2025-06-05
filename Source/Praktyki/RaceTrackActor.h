// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomSplineMeshActor.h"
#include "RaceTrackActor.generated.h"

class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;

UCLASS()
class PRAKTYKI_API ARaceTrackActor : public ACustomSplineMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARaceTrackActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
