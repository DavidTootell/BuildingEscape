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
	UE_LOG( LogTemp, Warning, TEXT( "Grab key pressed" ) )

	//LINE TRACE and reach any actors with physics body collions channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

		//If we hit something, attach a physics handle
	if (ActorHit)
	{
		//Attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true); // true == allow rotation.
	}
}

void UGrabber::Release()
{
	UE_LOG( LogTemp, Warning, TEXT( "Grab key released." ) )
		PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// if physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//Get play viewpoint this tick
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation,
			OUT PlayerViewPointRotation
		);
		// Calculate trace end
		FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

		// move object we're holding every frame
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		
}

	///Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//TODO Deal with collisons.
	}
	else
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
		UE_LOG( LogTemp, Warning, TEXT( "Input component successfully found on %s" ), *GetOwner()->GetName() )

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
	//Get play viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	// Calculate trace end
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

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
		UE_LOG( LogTemp, Warning, TEXT( "Actor Hit: %s" ), *(ActorHit->GetName()) )
	}

	return Hit;
}
