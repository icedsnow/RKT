// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RKT : ModuleRules
{
	public RKT(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "CableComponent" });

        PublicIncludePaths.AddRange(new string[] { "CableComponent/Public", "CableComponent/Classes" });

        PublicDependencyModuleNames.AddRange(new string[] { "Landscape" });
    }
}
