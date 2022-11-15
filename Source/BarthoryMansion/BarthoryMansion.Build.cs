// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BarthoryMansion : ModuleRules
{
    public BarthoryMansion(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        //"MovieScene"
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "UMG", "LevelSequence", "Niagara", "MovieScene", "FMODStudio" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}