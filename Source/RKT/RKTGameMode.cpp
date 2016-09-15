// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RKT.h"
#include "RKTGameMode.h"
#include "RKTPawn.h"

ARKTGameMode::ARKTGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ARKTPawn::StaticClass();
}
