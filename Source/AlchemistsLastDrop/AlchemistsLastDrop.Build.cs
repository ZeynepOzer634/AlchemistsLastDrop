// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AlchemistsLastDrop : ModuleRules
{
	public AlchemistsLastDrop(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AlchemistsLastDrop",
			"AlchemistsLastDrop/Variant_Platforming",
			"AlchemistsLastDrop/Variant_Platforming/Animation",
			"AlchemistsLastDrop/Variant_Combat",
			"AlchemistsLastDrop/Variant_Combat/AI",
			"AlchemistsLastDrop/Variant_Combat/Animation",
			"AlchemistsLastDrop/Variant_Combat/Gameplay",
			"AlchemistsLastDrop/Variant_Combat/Interfaces",
			"AlchemistsLastDrop/Variant_Combat/UI",
			"AlchemistsLastDrop/Variant_SideScrolling",
			"AlchemistsLastDrop/Variant_SideScrolling/AI",
			"AlchemistsLastDrop/Variant_SideScrolling/Gameplay",
			"AlchemistsLastDrop/Variant_SideScrolling/Interfaces",
			"AlchemistsLastDrop/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
