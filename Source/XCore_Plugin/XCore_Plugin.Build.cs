// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XCore_Plugin : ModuleRules
{
	public XCore_Plugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
