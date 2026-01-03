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
				"GameplayTags",
				"PhysicsCore",
				"DeveloperSettings"
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
				"AudioExtensions"
			}
		);
		
		// WICHTIG: Diese Abhängigkeiten erlauben uns, Assets zu erstellen (nur im Editor)
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd",
					"AssetTools",
					"EditorFramework",
					"AudioEditor" 
				}
			);
		}
	}
}