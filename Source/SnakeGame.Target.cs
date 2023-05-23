// Snake Game, Copyright LifeEXE. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SnakeGameTarget : TargetRules
{
    public SnakeGameTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "SnakeGame" });
    }
}
