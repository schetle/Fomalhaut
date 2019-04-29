workspace "Fomalhaut"
    location "build"
    configurations { "Debug", "Release" }
    
project "Fomalhaut"
    location "build"
    configurations { "Debug", "Release" }
    kind "StaticLib"
    language "C++"
    targetdir "build/lib/%{cfg.buildcfg}"
    libdirs {"lib"}
    includedirs {"include", "src"}
    defines { "NOMINMAX" }

    files {
        "include/**.h",
        "include/**.hpp",
        "src/**.h",
        "src/**.cpp",
    }

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
        architecture "x64"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
        architecture "x64"

project "Fomalhaut_Tests"
    location "build"
    configurations { "Debug", "Release" }
    kind "ConsoleApp"
    language "C++"
    targetdir "build/bin/tests/%{cfg.buildcfg}"
    libdirs {"lib", "build/lib"}
    includedirs {"include", "src"}

    files {
        "include/**.h",
        "include/**.hpp",
        "src/**.h",
        "src/**.cpp",
        "test/**.h",
        "test/**.hpp",
        "test/**.cpp"
    }



