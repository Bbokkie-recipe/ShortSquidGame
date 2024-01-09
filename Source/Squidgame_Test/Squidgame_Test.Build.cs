// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Squidgame_Test : ModuleRules
{
	public Squidgame_Test(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
