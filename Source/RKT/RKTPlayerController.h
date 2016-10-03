// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RKTPawn.h"
#include "GameFramework/PlayerController.h"
#include "RKTPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RKT_API ARKTPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
private:
	ARKTPawn* GetControlledRocket() const;
	
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

};
