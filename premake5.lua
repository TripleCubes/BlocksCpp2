workspace "Blocks"
    configurations { "Debug", "Release", }
    location "_Premake"

project "Blocks"
    filter "configurations:Debug"
        kind "ConsoleApp"
    filter "configurations:Release"
        kind "WindowedApp"
    filter {}

    language "C++"
    architecture "x86_64"

    targetdir "_Build/Bin/%{cfg.buildcfg}"
    objdir "_Build/Objs/%{cfg.buildcfg}"

    includedirs {
        "_Libs/glfw-3.3.8/include/", 
        "_Libs/glad/include/",
        "_Libs/GLM-0.9.9.8/g-truc-glm-bf71a83/",
        "_Libs/Stb_image/include/",
        "_Libs/FastNoiseLite-master-95900f7/",
        "_Libs/freetype-2.13.0/include/",
    }

    files { "Codes/**.cpp", "_Libs/Stb_image/Codes/stb_image.cpp", }

    buildoptions { "-Wall" }
    linkoptions { "-static", "-static-libgcc", "-static-libstdc++", }

    links { "gdi32", "GLFW", "GLAD", "FreeType", }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"

LIB_BINDIR = "_Libs/_Build/%{prj.name}/Bin/%{cfg.buildcfg}"
LIB_OBJDIR = "_Libs/_Build/%{prj.name}/Objs/%{cfg.buildcfg}"

project "GLFW"
    kind "StaticLib"
    language "C"
    architecture "x86_64"

    targetdir(LIB_BINDIR)
    objdir(LIB_OBJDIR)

    files {
        "_Libs/glfw-3.3.8/src/glfw_config.h",
        "_Libs/glfw-3.3.8/include/GLFW/glfw3.h",
        "_Libs/glfw-3.3.8/include/GLFW/glfw3native.h",

        "_Libs/glfw-3.3.8/src/context.c",
        "_Libs/glfw-3.3.8/src/init.c",
        "_Libs/glfw-3.3.8/src/input.c",
        "_Libs/glfw-3.3.8/src/monitor.c",
        "_Libs/glfw-3.3.8/src/vulkan.c",
        "_Libs/glfw-3.3.8/src/window.c",
    }

    filter "system:windows"
        files {
            "_Libs/glfw-3.3.8/src/win32_platform.h", 
            "_Libs/glfw-3.3.8/src/win32_joystick.h",
            "_Libs/glfw-3.3.8/src/wgl_context.h", 
            "_Libs/glfw-3.3.8/src/egl_context.h", 
            "_Libs/glfw-3.3.8/src/osmesa_context.h",

            "_Libs/glfw-3.3.8/src/win32_init.c", 
            "_Libs/glfw-3.3.8/src/win32_joystick.c",
            "_Libs/glfw-3.3.8/src/win32_monitor.c", 
            "_Libs/glfw-3.3.8/src/win32_time.c", 
            "_Libs/glfw-3.3.8/src/win32_thread.c", 
            "_Libs/glfw-3.3.8/src/win32_window.c",
            "_Libs/glfw-3.3.8/src/wgl_context.c", 
            "_Libs/glfw-3.3.8/src/egl_context.c", 
            "_Libs/glfw-3.3.8/src/osmesa_context.c",
        }

        defines { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS",
        }

    filter {}

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

project "GLAD"
    kind "StaticLib"
    language "C"
    architecture "x86_64"

    targetdir(LIB_BINDIR)
    objdir(LIB_OBJDIR)

    includedirs { "_Libs/glad/include/" }

    files { "_Libs/glad/src/glad.c" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

project "FreeType"
    kind "StaticLib"

    language "C"
    architecture "x86_64"

    targetdir(LIB_BINDIR)
    objdir(LIB_OBJDIR)

    includedirs { "_Libs/freetype-2.13.0/include/" }

    files { 
        "_Libs/freetype-2.13.0/src/autofit/autofit.c",
        "_Libs/freetype-2.13.0/src/base/ftbase.c",
        "_Libs/freetype-2.13.0/src/base/ftbbox.c",
        "_Libs/freetype-2.13.0/src/base/ftbdf.c",
        "_Libs/freetype-2.13.0/src/base/ftbitmap.c",
        "_Libs/freetype-2.13.0/src/base/ftcid.c",
        "_Libs/freetype-2.13.0/src/base/ftfstype.c",
        "_Libs/freetype-2.13.0/src/base/ftgasp.c",
        "_Libs/freetype-2.13.0/src/base/ftglyph.c",
        "_Libs/freetype-2.13.0/src/base/ftgxval.c",
        "_Libs/freetype-2.13.0/src/base/ftinit.c",
        "_Libs/freetype-2.13.0/src/base/ftmm.c",
        "_Libs/freetype-2.13.0/src/base/ftotval.c",
        "_Libs/freetype-2.13.0/src/base/ftpatent.c",
        "_Libs/freetype-2.13.0/src/base/ftpfr.c",
        "_Libs/freetype-2.13.0/src/base/ftstroke.c",
        "_Libs/freetype-2.13.0/src/base/ftsynth.c",
        "_Libs/freetype-2.13.0/src/base/fttype1.c",
        "_Libs/freetype-2.13.0/src/base/ftwinfnt.c",
        "_Libs/freetype-2.13.0/src/bdf/bdf.c",
        "_Libs/freetype-2.13.0/src/bzip2/ftbzip2.c",
        "_Libs/freetype-2.13.0/src/cache/ftcache.c",
        "_Libs/freetype-2.13.0/src/cff/cff.c",
        "_Libs/freetype-2.13.0/src/cid/type1cid.c",
        "_Libs/freetype-2.13.0/src/gzip/ftgzip.c",
        "_Libs/freetype-2.13.0/src/lzw/ftlzw.c",
        "_Libs/freetype-2.13.0/src/pcf/pcf.c",
        "_Libs/freetype-2.13.0/src/pfr/pfr.c",
        "_Libs/freetype-2.13.0/src/psaux/psaux.c",
        "_Libs/freetype-2.13.0/src/pshinter/pshinter.c",
        "_Libs/freetype-2.13.0/src/psnames/psnames.c",
        "_Libs/freetype-2.13.0/src/raster/raster.c",
        "_Libs/freetype-2.13.0/src/sdf/sdf.c",
        "_Libs/freetype-2.13.0/src/sfnt/sfnt.c",
        "_Libs/freetype-2.13.0/src/smooth/smooth.c",
        "_Libs/freetype-2.13.0/src/svg/svg.c",
        "_Libs/freetype-2.13.0/src/truetype/truetype.c",
        "_Libs/freetype-2.13.0/src/type1/type1.c",
        "_Libs/freetype-2.13.0/src/type42/type42.c",
        "_Libs/freetype-2.13.0/src/winfonts/winfnt.c",        
    }

    filter "system:windows"
        files {
            "_Libs/freetype-2.13.0/builds/windows/ftsystem.c",

            "_Libs/freetype-2.13.0/builds/windows/ftdebug.c",
            "_Libs/freetype-2.13.0/src/base/ftver.rc",
        }

    filter {}

    defines { "FT2_BUILD_LIBRARY" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
