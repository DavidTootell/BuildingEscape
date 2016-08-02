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
	ClosedRotation = Owner->GetActorRotation();
	
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation( FRotator( 0.0f, OpenAngle, 0.0f ));
}


void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation( ClosedRotation );
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() > 30.f) // TODO make into a parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it is time to close the door
	
	CurrentTime = GetWorld()->GetTimeSeconds();

	if ((CurrentTime - LastDoorOpenTime) > DoorCloseDelay )
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// find overlapping actors
	if (!PressurePlate) { UE_LOG( LogTemp, Error, TEXT( "No pressure plate attached." ) ) return 0.0f; }; // Check pointer is not nullptr
	TArray <AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors( OUT OverlappingActors );

	for (const auto& Actor : OverlappingActors)
	{
		//Get actor mass and add masses together
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
	}


	return TotalMass;
}

