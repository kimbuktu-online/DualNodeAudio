// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DualNodeAudio : ModuleRules
{
	public DualNodeAudio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


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
		        "NetCore"
	        }
        );
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
