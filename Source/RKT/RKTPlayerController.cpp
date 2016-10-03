// Fill out your copyright notice in the Description page of Project Settings.

#include "RKT.h"
#include "RKTPlayerController.h"

void ARKTPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ARKTPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ARKT* ARKTPlayerController::GetControlledRocket() const
{
	return Cast<ARKT>(GetPawn());
}
