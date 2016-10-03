// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RKT.h"
#include "RKTPawn.h"

ARKT::ARKT()
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
	MaxPitch = 15.f;
	MinPitch = -10.f;
	
	///**********CONSTRAIN ROLL
	MaxRoll = 0.f;
	MinRoll = 0.f;
}

void ARKT::Tick(float DeltaSeconds)
{
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);

	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

	// Move Rocket forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);
	
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

	///*********MAXPITCH
	DeltaRotation.Pitch = FMath::ClampAngle(CurrentPitchSpeed * DeltaSeconds, MinDeltaPitch, MaxDeltaPitch);
	///********MAXPITCH
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	///DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;
	///********CONSTRAIN ROLL
	DeltaRotation.Roll = FMath::ClampAngle(CurrentRollSpeed * DeltaSeconds, MinDeltaRoll, MaxDeltaRoll);
	///********CONSTRAIN ROLL
	// Rotate Rocket
	AddActorLocalRotation(DeltaRotation);

}

void ARKT::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation(RootComponent);
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}


void ARKT::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this, &ARKT::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &ARKT::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARKT::MoveRightInput);
	//
}

void ARKT::ThrustInput(float Val)
{
	// Is there no input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ARKT::MoveUpInput(float Val)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ARKT::MoveRightInput(float Val)
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

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}