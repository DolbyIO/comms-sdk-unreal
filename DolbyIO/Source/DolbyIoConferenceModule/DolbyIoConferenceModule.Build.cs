using UnrealBuildTool;

public class DolbyIoConferenceModule : ModuleRules
{
    public DolbyIoConferenceModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "SdkAccessModule",
            }
            );
    }
}
