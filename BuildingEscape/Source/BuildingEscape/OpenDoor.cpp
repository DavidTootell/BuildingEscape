// Copyright ® David Tootell 2016. All rights reserved.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (!PressurePlate)
	{
		UE_LOG( LogTemp, Error, TEXT( "No pressure plate attached to %s"), *GetOwner()->GetName() )
	}
	
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() > TriggerMass ) // TODO make into a parameter
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	
	if (!PressurePlate) { return 0.f; }; // Check pointer is not nullptr
	// find overlapping actors
	TArray <AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors( OUT OverlappingActors );
	for (const auto& Actor : OverlappingActors)
	{
		//Get actor mass and add masses together
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

