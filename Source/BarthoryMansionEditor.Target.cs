// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BarthoryMansionEditorTarget : TargetRules
{
	public BarthoryMansionEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bUseUnityBuild = false;
		ExtraModuleNames.Add("BarthoryMansion");
	}
}
