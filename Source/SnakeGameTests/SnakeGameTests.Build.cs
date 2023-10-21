// Snake Game, Copyright LifeEXE. All Rights Reserved.

using UnrealBuildTool;

public class SnakeGameTests : ModuleRules
{
    public SnakeGameTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "SnakeGame" });
        PublicIncludePaths.AddRange(new string[] { "SnakeGameTests" });

        if (Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
            PublicDependencyModuleNames.Add("FunctionalTesting");
        }
    }
}
