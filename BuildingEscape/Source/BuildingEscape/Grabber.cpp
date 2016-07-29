// Copyright ® David Tootell 2016. All rights reserved.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT // Annotaion to denote a side-effect will be returned from variables passed into function.

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG( LogTemp, Warning, TEXT( "Grabber Initialized." ) );	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Get play viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);


	/*UE_LOG( LogTemp, Warning, TEXT( "Location: %s, Rotation: %s" ), 
		*PlayerViewPointLocation.ToString(), 
		*PlayerViewPointRotation.ToString() 
	)*/

	// Calculate trace end
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	
	//Draw a red trace in world to visualise
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor( 255, 0, 0 ),
		false,
		0.f,
		0.f,
		10.f
		);

	//Ray-cast out to reach distance

	//See what we hit
}

