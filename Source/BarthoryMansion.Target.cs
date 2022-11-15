// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BarthoryMansionTarget : TargetRules
{
    public BarthoryMansionTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        bUseUnityBuild = false;
        ExtraModuleNames.Add("BarthoryMansion");
    }
}

