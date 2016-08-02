// Copyright ® David Tootell 2016. All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


private:
	UPROPERTY( EditAnywhere )
		float OpenAngle = 90.0f;

	UPROPERTY( EditAnywhere )
		ATriggerVolume* PressurePlate = nullptr;
	
	UPROPERTY( EditAnywhere )
		float DoorCloseDelay = 1.0f;

		float LastDoorOpenTime;

	UPROPERTY( VisibleAnywhere )
		FRotator ClosedRotation; // Used in BeginPlay() to set initial rotation.

	// the owning door
	AActor* Owner = nullptr; 

	float CurrentTime = 0.0f;

	float GetTotalMassOfActorsOnPlate(); //Returns in KG

};
