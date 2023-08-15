-- premake5.lua
workspace "Workspace"
   configurations
   {
      "Debug",
      "Release"
   }

   outputdir = "%{cfg.buildcfg}"

   project "RotR_Colorfix"
      filename "RotR_Colorfix"
      language "C"
      kind "ConsoleApp"

      targetdir ("bin/" .. outputdir .. "/%{prj.name}")
      objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

      -- *.c all c in premake5.lua directory, **.c recursively all .c files from directory down
      files
      {
         "src/**.h",
         "src/**.c"
      }

      includedirs 
      {
         "src/",
         "vendor/stb_image_read_resize_write/"
      }

      links
      {
      }

      filter "configurations:Debug"
         defines { "DEBUG" }
         symbols "On"

      filter "configurations:Release"
         -- defines { "NDEBUG" }
         optimize "On"

