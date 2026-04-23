// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPProg1Obligatorio : ModuleRules
{
	public TPProg1Obligatorio(ReadOnlyTargetRules Target) : base(Target)
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
			"TPProg1Obligatorio",
			"TPProg1Obligatorio/Variant_Platforming",
			"TPProg1Obligatorio/Variant_Platforming/Animation",
			"TPProg1Obligatorio/Variant_Combat",
			"TPProg1Obligatorio/Variant_Combat/AI",
			"TPProg1Obligatorio/Variant_Combat/Animation",
			"TPProg1Obligatorio/Variant_Combat/Gameplay",
			"TPProg1Obligatorio/Variant_Combat/Interfaces",
			"TPProg1Obligatorio/Variant_Combat/UI",
			"TPProg1Obligatorio/Variant_SideScrolling",
			"TPProg1Obligatorio/Variant_SideScrolling/AI",
			"TPProg1Obligatorio/Variant_SideScrolling/Gameplay",
			"TPProg1Obligatorio/Variant_SideScrolling/Interfaces",
			"TPProg1Obligatorio/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
