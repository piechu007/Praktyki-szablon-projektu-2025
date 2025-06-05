// Copyright 2025 Teyon. All Rights Reserved.

#include "CustomSplineMeshActor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ACustomSplineMeshActor::ACustomSplineMeshActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    TrackSpline = CreateDefaultSubobject<USplineComponent>(TEXT("TrackSpline"));
    RootComponent = TrackSpline;
}

void ACustomSplineMeshActor::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);
    GenerateNewMeshes();
}

void ACustomSplineMeshActor::GenerateNewMeshes()
{
    if (!RoadMesh || !TrackSpline)
    {
        return;
    }

    ClearOldMeshes();

    FVector StartPos = TrackSpline->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::Local);
    FVector StartTangent = MeshLength *(TrackSpline->GetTangentAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::Local)).GetSafeNormal();
    FVector EndPos;
    FVector EndTangent;
    float CurrentDistance = MeshLength;

    const int32 NumMeshes = TrackSpline->GetSplineLength() / MeshLength;
    for (int32 i = 0; i <= NumMeshes; ++i)
    {
        EndPos = TrackSpline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::Local);
        EndTangent = (TrackSpline->GetTangentAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::Local)).GetSafeNormal() * MeshLength;

        USplineMeshComponent *SplineMesh = NewObject<USplineMeshComponent>(this);
        if (SplineMesh)
        {
            SplineMesh->SetStaticMesh(RoadMesh);
            SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
            SplineMesh->SetStartScale(FVector2D(MeshScale.Y, MeshScale.Z));
            SplineMesh->SetEndScale(FVector2D(MeshScale.Y, MeshScale.Z));
            SplineMesh->SetForwardAxis(ESplineMeshAxis::X);

            SplineMesh->SetMobility(EComponentMobility::Static);
            SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            SplineMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

            SplineMesh->RegisterComponentWithWorld(GetWorld());
            SplineMesh->AttachToComponent(TrackSpline, FAttachmentTransformRules::KeepRelativeTransform);
            SpawnedMeshes.Add(SplineMesh);
        }

        CurrentDistance += MeshLength;
        StartPos = EndPos;
        StartTangent = EndTangent;
    }
}

void ACustomSplineMeshActor::ClearOldMeshes()
{
    for (USplineMeshComponent *Mesh : SpawnedMeshes)
    {
        if (Mesh)
        {
            Mesh->DestroyComponent();
        }
    }
    SpawnedMeshes.Empty();
}
