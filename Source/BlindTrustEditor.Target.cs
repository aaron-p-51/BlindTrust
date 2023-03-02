// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class BlindTrustEditorTarget : TargetRules
{
	public BlindTrustEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "BlindTrust" } );

        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
    }
}
