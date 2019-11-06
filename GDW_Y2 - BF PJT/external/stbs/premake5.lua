project "Stbs"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "stb_image.h",
        "stb_image_write.h",
        "stb_include.h",
        "stb_perlin.h",
        "stb_rect_pack.h",
        "stb_truetype.h",
        "stb_impl.cpp",
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"