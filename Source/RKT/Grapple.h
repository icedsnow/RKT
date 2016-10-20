// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "RKTPawn.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
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


private:

	
	float Reach = 1000.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	// Find attached physics Handle
	void FindPhysicsHandleComponent();

	// Setup attached input component
	void SetupInputComponent();



protected:
	// Ray-cast and grab what's in reach
	void Grab();

	// Called when grab is released
	void Release();


public:
// 	
// 	TSubclassOf<UBlueprint> GrappleBP;

///Spawn Grapple, Hopefully
// 	UPROPERTY(Category = Grapple, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
// 		TSubclassOf<class UGrapple> GrappleBP;
		
		//class UBlueprint* GrappleBP;

	//FORCEINLINE class UBlueprint* GetGrappleBP() const { return GrappleBP; }






	//SpawnActor from blueprint
// 	template <typename ASpawnBP>
// 	FORCEINLINE ASpawnBP* SpawnBP(
// 		UWorld* TheWorld,
// 		UClass* TheBP,
// 		const FVector& Loc,
// 		const FRotator& Rot,
// 		const bool bNoCollisionFail = true,
// 		AActor* Owner = NULL,
// 		APawn* Instigator = NULL
// 	)
// 	{
// 		if (!TheWorld) return NULL;
// 		if (!TheBP) return NULL;
// 		//~~~~~~~~~~~
// 
// 		FActorSpawnParameters SpawnInfo;
// 		SpawnInfo.bNoCollisionFail = bNoCollisionFail;
// 		SpawnInfo.Owner = Owner;
// 		SpawnInfo.Instigator = Instigator;
// 		SpawnInfo.bDeferConstruction = false;
// 
// 		return TheWorld->SpawnActor<ASpawnBP>(TheBP, Loc, Rot, SpawnInfo);
// 	}




	
};
