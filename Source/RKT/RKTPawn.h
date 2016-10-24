// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
//#include "CableComponent.h"
#include "RKTPawn.generated.h"


UCLASS(config=Game)
class RKT_API ARKTPawn : public APawn
{
	GENERATED_BODY()

	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RocketMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:

	// Sets default values for this pawn's properties
	ARKTPawn();
	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides

	//class URKTFloatingPawnMovement* RKTMovementComponent;
	class URKTFloatingPawnMovement* RKTMovementComponent;

	virtual UPawnMovementComponent* GetMovementComponent() const override;


protected:

	const FHitResult GetFirstPhysicsBodyInReach();

	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides

	/** Bound to the thrust axis */
	void ThrustInput(float Val);
	
	/** Bound to the vertical axis */
	void MoveUpInput(float Val);

	/** Bound to the horizontal axis */
	void MoveRightInput(float Val);



private:

	///NEW
	

	/** How quickly forward speed changes */
	UPROPERTY(Category = Rocket, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Acceleration;

	UPROPERTY(Category = Rocket, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector Velocity;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Rocket, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Rocket, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Rocket, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float MinSpeed;

	///**********MAXPITCH
	UPROPERTY(Category = Pitch, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float MaxPitch;

	UPROPERTY(Category = Pitch, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float MinPitch;

	///**********CONSTRAIN ROLL
	UPROPERTY(Category = Rocket, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float MaxRoll;

	UPROPERTY(Category = Rocket, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float MinRoll;

	/** Current forward speed */
	UPROPERTY(Category = Rocket, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;
	
	
	

public:
	/** Returns RocketMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetRocketMesh() const { return RocketMesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

};
