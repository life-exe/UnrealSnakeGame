// Snake Game, Copyright LifeEXE. All Rights Reserved.

using UnrealBuildTool;

public class SnakeGame : ModuleRules
{
    public SnakeGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "EnhancedInput", "Niagara", "SlateCore" });
        PublicIncludePaths.AddRange(new string[] { "SnakeGame" });
    }
}
