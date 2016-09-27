// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "RKTPawn.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "CableComponent.h"
#include "Grapple.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RKT_API UGrapple : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrapple();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	
	///*********CABLE
///	UPROPERTY(Category = Cable, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
///		class UCableComponent* GPCable;
	
	
	///*********CABLE


private:

	
	float Reach = 1000.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	//UCableComponent* CableComponentLeft = nullptr;
	
	
	//void FindCableComponent();

	// Find attached physics Handle
	void FindPhysicsHandleComponent();

	// Setup attached input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Returns start of reach line
	FVector GetReachLineStart();
	// Returns current end of reach line
	FVector GetReachLineEnd();



protected:
	// Ray-cast and grab what's in reach
	void Grab();

	// Called when grab is released
	void Release();


public:

	///FORCEINLINE class UCableComponent* GetCableComponent() const { return GPCable; }
};
