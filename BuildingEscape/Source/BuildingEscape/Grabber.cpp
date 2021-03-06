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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG( LogTemp, Warning, TEXT( "Grabber Initialized." ) );

	FindPhysicsHandleComponent();
	SetupInputComponent();

}

void UGrabber::Grab()
{
	//LINE TRACE and reach any actors with physics body collions channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	//If we hit something, attach a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true); // true == allow rotation.
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; };
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!PhysicsHandle) { return; };
	// if physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// move object we're holding every frame
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}		
}

	///Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG( LogTemp, Error, TEXT( "PhysicsHandle component missing on %s" ), *GetOwner()->GetName() )
	}
}

void UGrabber::SetupInputComponent()
{
	///Look for Input Component
	InputComponet = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponet)
	{
		///Bind the input axis
		InputComponet->BindAction( "Grab", IE_Pressed, this, &UGrabber::Grab );
		InputComponet->BindAction( "Grab", IE_Released, this, &UGrabber::Release );
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "Input component missing on %s" ), *GetOwner()->GetName() )
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//Line Trace (Ray-cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ), false, GetOwner() );
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
		TraceParameters
	);

	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	//Get play viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

//Finds the playerviewpoint, calculates a line trace from viewpoint and returns line trace end
FVector UGrabber::GetReachLineEnd()
{
	//Get play viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	// Calculate trace end
	return (PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach));
}