// Copyright 2023 Dolby Laboratories

using System.IO;
using UnrealBuildTool;

public class DolbyIO : ModuleRules
{
    public DolbyIO(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        CppStandard = CppStandardVersion.Cpp17;
        bEnableExceptions = true;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "CoreUObject", "Engine", "HTTP", "Json", "Projects", "RenderCore", "RHI" });

        string ReleaseDir = "sdk-release";
        if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            ReleaseDir += "-ubuntu-20.04-clang10-libc++10";
        }
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            ReleaseDir += "-android";
        }
        string SdkDir = Path.Combine("$(PluginDir)", ReleaseDir);
        PublicIncludePaths.Add(Path.Combine(SdkDir, "include"));
        string LibDir = Path.Combine(SdkDir, "lib");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.AddRange(new string[] {
                Path.Combine(LibDir, "dolbyio_comms_media.lib"),
                Path.Combine(LibDir, "dolbyio_comms_sdk.lib"),
                Path.Combine(LibDir, "video_processor.lib")
            });

            string[] Dlls = new string[] { "avutil-57.dll",
                                           "avcodec-59.dll",
                                           "dvclient.dll",
                                           "dolbyio_comms_media.dll",
                                           "dolbyio_comms_sdk.dll",
                                           "opencv_core451.dll",
                                           "opencv_imgproc451.dll",
                                           "opencv_imgcodecs451.dll",
                                           "dvdnr.dll",
                                           "dlb_vidseg_c_api.dll",
                                           "video_processor.dll" };
            PublicDelayLoadDLLs.AddRange(Dlls);

            string BinDir = Path.Combine(SdkDir, "bin");
            foreach (string Dll in Dlls)
            {
                RuntimeDependencies.Add(Path.Combine(BinDir, Dll));
            }
            RuntimeDependencies.Add(Path.Combine(BinDir, "cac_dvdnr.dll"));
            RuntimeDependencies.Add(Path.Combine(BinDir, "model.dnr"));
            RuntimeDependencies.Add(Path.Combine(BinDir, "video_processor.model"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            string[] Libs = new string[] { Path.Combine(LibDir, "libdolbyio_comms_media.dylib"),
                                           Path.Combine(LibDir, "libdolbyio_comms_sdk.dylib"),
                                           Path.Combine(LibDir, "libvideo_processor.dylib") };
            PublicAdditionalLibraries.AddRange(Libs);
            PublicDelayLoadDLLs.AddRange(Libs);

            foreach (string Lib in Libs)
            {
                RuntimeDependencies.Add(Lib);
            }
            RuntimeDependencies.Add(Path.Combine(LibDir, "libcac_dvdnr.dylib"));
            RuntimeDependencies.Add(Path.Combine(LibDir, "libdlb_vidseg_c_api.dylib"));
            RuntimeDependencies.Add(Path.Combine(LibDir, "libdvclient.dylib"));
            RuntimeDependencies.Add(Path.Combine(LibDir, "libdvdnr.dylib"));
            RuntimeDependencies.Add(Path.Combine(LibDir, "libopencv_core.4.5.dylib"));
            RuntimeDependencies.Add(Path.Combine(LibDir, "libopencv_imgcodecs.4.5.dylib"));
            RuntimeDependencies.Add(Path.Combine(LibDir, "libopencv_imgproc.4.5.dylib"));
            RuntimeDependencies.Add(Path.Combine(LibDir, "model.dnr"));
            RuntimeDependencies.Add(Path.Combine(LibDir, "video_processor.model"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            string[] Libs = new string[] { Path.Combine(LibDir, "libavutil.so.57"),
                                           Path.Combine(LibDir, "libavcodec.so.59"),
                                           Path.Combine(LibDir, "libavformat.so.59"),
                                           Path.Combine(LibDir, "libdvclient.so"),
                                           Path.Combine(LibDir, "libdolbyio_comms_media.so"),
                                           Path.Combine(LibDir, "libdolbyio_comms_sdk.so") };
            PublicAdditionalLibraries.AddRange(new string[] {
                Path.Combine(LibDir, "libdolbyio_comms_media.so"),
                Path.Combine(LibDir, "libdolbyio_comms_sdk.so"),
            });
            PublicDelayLoadDLLs.AddRange(Libs);

            foreach (string Lib in Libs)
            {
                RuntimeDependencies.Add(Lib);
            }
            RuntimeDependencies.Add(Path.Combine(LibDir, "libcac_dvdnr.so"));
            RuntimeDependencies.Add(Path.Combine(LibDir, "model.dnr"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            LibDir = Path.Combine(SdkDir, "libs/android.arm64-v8a");

            // since sdk & media are also provided by the regular app, just to make sure
            // that nothing is duplicated in some way, we just need to "register" them
            // for the compilation part. Not the actual package. Right now we just duplicate

            string[] Libs = new string[] {
                Path.Combine(LibDir, "libdolbyio_comms_sdk.so"),
                Path.Combine(LibDir, "libdolbyio_comms_media.so"),
                Path.Combine(LibDir, "libdolbyio_comms_sdk_android_cppsdk.so")
            };

            PublicAdditionalLibraries.AddRange(new string[] {
                Path.Combine(LibDir, "libdolbyio_comms_sdk.so"),
                Path.Combine(LibDir, "libdolbyio_comms_media.so"),
                Path.Combine(LibDir, "libdolbyio_comms_sdk_android_cppsdk.so")
            });
            PublicDelayLoadDLLs.AddRange(Libs);

            foreach (string Lib in Libs)
            {
                RuntimeDependencies.Add(Lib);
            }

            AdditionalPropertiesForReceipt.Add(
                "AndroidPlugin",
                Path.Combine(Utils.MakePathRelativeTo(PluginDirectory, Target.RelativeEnginePath), "DolbyIO.Android.xml"));
        }
    }
}
