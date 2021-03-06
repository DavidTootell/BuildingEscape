// Copyright ® David Tootell 2016. All rights reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FDoorEvent );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY( BlueprintAssignable )
		FDoorEvent OnOpen;

	UPROPERTY( BlueprintAssignable )
		FDoorEvent OnClose;

private:
	UPROPERTY( EditAnywhere )
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY( EditAnywhere )
		float TriggerMass = 30.0f; //Mass required on pressureplate to trigger

	// the owning door
	AActor* Owner = nullptr; 

	float CurrentTime = 0.0f;

	float GetTotalMassOfActorsOnPlate(); //Returns in KG

};
