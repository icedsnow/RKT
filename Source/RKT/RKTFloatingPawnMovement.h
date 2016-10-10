// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RKTPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "RKTFloatingPawnMovement.generated.h"

/**
 * 
 */
UCLASS()
class RKT_API URKTFloatingPawnMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaSeconds, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	float DeflectionSlide(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit);
};
