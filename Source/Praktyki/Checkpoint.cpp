// Copyright 2025 Teyon. All Rights Reserved.


#include "Checkpoint.h"
#include "Components/BoxComponent.h"
#include "PlayerVehiclePawn.h"


// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(BoxCollision);

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

    BoxCollision->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::HandleComponentBeginOverlap);
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> Actors;
    GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
    {
		FString Name = Actor->GetActorNameOrLabel();
    	UE_LOG(LogTemp, Display, TEXT("Tick-GetOverlappingActors(): %s"), *Name);
    }
}

void ACheckpoint::HandleComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerVehiclePawn *PlayerVehiclePawn = Cast<APlayerVehiclePawn>(OtherActor);
	if (PlayerVehiclePawn)
	{
		CheckpointReached(PlayerVehiclePawn);
	}
}

void CheckpointReached(APlayerVehiclePawn *PlayerVehiclePawn)
{
	
}


