// Copyright 2025 Teyon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomSplineMeshActor.generated.h"

class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;

UCLASS()
class PRAKTYKI_API ACustomSplineMeshActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACustomSplineMeshActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componts")
	USplineComponent *TrackSpline;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Track")
	UStaticMesh *RoadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Track")
	FVector MeshScale = FVector(1.f, 1.f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Track")
	float MeshLength = 600.f;

private:
	void GenerateNewMeshes();
	void ClearOldMeshes();

	TArray<USplineMeshComponent*> SpawnedMeshes;
};
