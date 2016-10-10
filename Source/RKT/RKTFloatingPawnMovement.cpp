// Fill out your copyright notice in the Description page of Project Settings.

#include "RKT.h"
#include "RKTPawn.h"
#include "RKTFloatingPawnMovement.h"



void URKTFloatingPawnMovement::TickComponent(float DeltaSeconds, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaSeconds, TickType, ThisTickFunction);
};

float URKTFloatingPawnMovement::DeflectionSlide(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit)
{
	const FVector OldHitNormal = Normal;

	FVector SlideDelta = ComputeSlideVector(Delta, Time, Normal, Hit);
	const FQuat Rotation = UpdatedComponent->GetComponentQuat();
	SafeMoveUpdatedComponent(SlideDelta, Rotation, true, Hit);

	//If you hit a SECOND wall, readjust, then slide along it
	if (Hit.IsValidBlockingHit())
	{
		TwoWallAdjust(SlideDelta, Hit, OldHitNormal);
		SafeMoveUpdatedComponent(SlideDelta, Rotation, true, Hit);

		//If you hit a THIRD wall, readjust, then slide along it
		if (Hit.IsValidBlockingHit())
		{
			TwoWallAdjust(SlideDelta, Hit, OldHitNormal);
			SafeMoveUpdatedComponent(SlideDelta, Rotation, true, Hit);

			//If you hit a FOURTH wall, readjust, then slide along it
			if (Hit.IsValidBlockingHit())
			{
				TwoWallAdjust(SlideDelta, Hit, OldHitNormal);
				SafeMoveUpdatedComponent(SlideDelta, Rotation, true, Hit);
			}
		}
	}
	return 0.f;
}