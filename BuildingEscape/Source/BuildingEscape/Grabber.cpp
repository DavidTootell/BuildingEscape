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

	///Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//TODO Deal with collisons.
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "PhysicsHandle component missing on %s"), *GetOwner()->GetName() )
	}

	///Look for Input Component
	InputComponet = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponet)
	{
		UE_LOG( LogTemp, Warning, TEXT( "Input component successfully found on %s" ), *GetOwner()->GetName() )
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "Input component missing on %s" ), *GetOwner()->GetName() )
	}
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

	//Setup Query parameters
	FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ), false, GetOwner() );
	
	//Line Trace (Ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
		TraceParameters
	);

	//See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG( LogTemp, Warning, TEXT( "Actor Hit: %s" ), *( ActorHit->GetName() ) ) 
	}

}

