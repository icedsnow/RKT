// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RKT.h"
#include "Runtime/Landscape/Classes/LandscapeProxy.h"
#include "Runtime/Landscape/Classes/Landscape.h"
#include "RKTPawn.h"
#include "RKTFloatingPawnMovement.h"
#include "EngineUtils.h"
#include "Engine/EngineTypes.h"
//#include "PhysicsVolume.generated.h"


ARKTPawn::ARKTPawn()
{
	///New
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Structure to hold one-time initialization
	
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> RocketMesh;
		FConstructorStatics()
			: RocketMesh(TEXT("/Game/Flying/Meshes/Rocket/Rocket_Pre_mesh.Rocket_Pre_mesh"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh0"));
	RocketMesh->SetStaticMesh(ConstructorStatics.RocketMesh.Get());
	RootComponent = RocketMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(-500.f,0.f,150.f);
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = 2000.f;
	CurrentForwardSpeed = 500.f;

	///***********MAXPITCH
	MaxPitch = 5.f;
	MinPitch = -5.f;
	
	///**********CONSTRAIN ROLL
	MaxRoll = 0.f;
	MinRoll = 0.f;


	///NEW - Movement Components
	RKTMovementComponent = CreateDefaultSubobject<URKTFloatingPawnMovement>(TEXT("CustomMovementComponent"));
	RKTMovementComponent->UpdatedComponent = RootComponent;

}

void ARKTPawn::Tick(float DeltaSeconds)
{
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);
	
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

	// Move Rocket forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true); //true
	
	///*********MAXPITCH
	const float OldPitch = GetActorRotation().Pitch;
	const float MinDeltaPitch = MinPitch - OldPitch;
	const float MaxDeltaPitch = MaxPitch - OldPitch;
	
	///********CONSTRAIN ROLL
	const float OldRoll = GetActorRotation().Roll;
	const float MinDeltaRoll = MinRoll - OldRoll;
	const float MaxDeltaRoll = MaxRoll - OldRoll;
	

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0,0,0);
	//OLD
	//DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	///
	
	DeltaRotation.Pitch = FMath::ClampAngle(CurrentPitchSpeed * DeltaSeconds, MinDeltaPitch, MaxDeltaPitch);
	//Yaw doesn't need clamp constraint
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;


	///DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;
	
	DeltaRotation.Roll = FMath::ClampAngle(CurrentRollSpeed * DeltaSeconds, MinDeltaRoll, MaxDeltaRoll);
	
	// Rotate Rocket
	//AddActorL(DeltaLocation);

	AddActorLocalRotation(DeltaRotation);



	






}

void ARKTPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{

	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	

	// Deflect along the surface when we collide. //0.025

	FRotator CurrentRotation = GetActorRotation(RootComponent);

	///SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
	//////////////////////////////////////////////////////////////////////////
	const FVector OldLocation = RootComponent->GetComponentLocation();
	const FQuat QRotation = RootComponent->GetComponentQuat();

	//RKTMovementComponent->SafeMoveUpdatedComponent(Delta, QRotation, true, Hit);
	// If we bumped into something, try to slide along it
	//if (Hit.IsValidBlockingHit())
	//{
		//RKTMovementComponent->HandleImpact(Hit, GetWorld()->GetDeltaSeconds(), Delta);
		//Try to slide along surface
		//RKTMovementComponent->DeflectionSlide(Delta, 1.f - Hit.Time, Hit.Normal, SlideHit);
		//RKTMovementComponent->SlideAlongSurface(Delta, 1.f, HitNormal, SlideHit, true);
		///
		//Log Fires correctly, Slide still doesn't work
		//UE_LOG(LogTemp, Warning, TEXT("Hit Result: %s"), *Hit.ToString());

		
	//}
	//Velocity formula
	//Update Velocity
	const FVector NewLocation = RootComponent->GetComponentLocation();
	Velocity = ((NewLocation - OldLocation) / GetWorld()->GetDeltaSeconds());
	FVector Delta = Velocity * GetWorld()->GetDeltaSeconds();
	//GetOwner()->GetParentActor()->FindComponentByClass(Grapple);

	
	auto HitActorName = Other->GetName();
	//////////////////////////////////////////////////////////////////////////
	
	UWorld* world = GetWorld();
	ALandscape* landscape = nullptr;
	AVolume* ceiling = nullptr;
	UClass* HitPtr = nullptr;
	FHitResult SlideHit = Hit;
	if (world != nullptr) {
		// Find the active landscape
		TActorIterator<ALandscape> landscapeIterator(world);
		landscape = *landscapeIterator;

		//Find the active blocking volume for ceiling
		TActorIterator<AVolume> physicsvolumeIterator(world);
		ceiling = *physicsvolumeIterator;
	}
	FString lsName = landscape->GetName();
	if (landscape != nullptr)
	{
		HitPtr = Other->GetClass();
		if (HitPtr == landscape->GetClass())
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *lsName);
	//		AddActorLocalOffset(HitNormal * 10);
		}
	
		//	AddActorLocalOffset((HitNormal * 5) * -1);
		
		else
		{
			if (HitPtr == ceiling->GetClass())
			{
				
			}
			else
			{
	//			SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.05f));
			}
			
		}
		//landscape->LandscapeMaterial->GetName();
		
	}
	//ALandscape* LandscapeClassPTR = nullptr;
	//LandscapeClassPTR->SetClass(landscape->GetClass()); //landscape->GetClass();
	/*
	if (Hit.Actor->IsA(landscape))
	{

	}
	*/
	
	
	
	
	
	//ALandscape* landscapeclass = nullptr;
	//landscapeclass = GetParentActor()->GetClass();
	//if (Other->GetActorClass()->IsA(Landscape::StaticClass());//  landscapeclass::StaticClass())
	//{ 
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *lsName);
//	}
	
	//////////////////////////////////////////////////////////////////////////
	//auto ECCIsStatic = Hit.GetActor()->GetComponentsCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic);
	//auto HitActor = Hit.GetActor();
	
	
		
	
	/*
	if (Hit.GetActor()->GetComponentsCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic))
	{
		//if (GetHName != landscape)
		{
			SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
		}
	}
	*/

}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////


///NEW Movement Component
UPawnMovementComponent* ARKTPawn::GetMovementComponent() const
{
	return RKTMovementComponent;
}
void ARKTPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this, &ARKTPawn::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &ARKTPawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARKTPawn::MoveRightInput);
}

void ARKTPawn::ThrustInput(float Val)
{
	// Is there no input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.1f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ARKTPawn::MoveUpInput(float Val)
{
	
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	
	

	// Target pitch speed is based in input
	float TargetPitchSpeed = bHasInput ? (Val * TurnSpeed) : (GetActorRotation().Pitch * -2.f);

	// When steering, we decrease pitch slightly
	//TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed));// * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	
}

void ARKTPawn::MoveRightInput(float Val)
{
	
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value
	///ROLL constrained min, max = 0
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate to target roll speed
	//CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	
}