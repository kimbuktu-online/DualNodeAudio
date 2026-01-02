// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DualNodeAudio : ModuleRules
{
	public DualNodeAudio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"GameplayTags", // WICHTIG: Für die Tags
				"PhysicsCore",  // WICHTIG: Für Physical Materials
				"DeveloperSettings" // WICHTIG: Für Project Settings
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"NetCore",
				"AudioExtensions" // Nützlich für Modulation/Occlusion
			}
		);
		
		// --- EDITOR ONLY DEPENDENCIES ---
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd",
					"AssetTools",
					"EditorFramework",
					"AudioEditor" // <--- FIX: Hier leben die Sound Factories!
				}
			);
		}
	}
}