-- premake5.lua
workspace "Dumb S3M Header Editor"
   configurations { "Debug", "Release" }

project "Dumb S3M Header Editor"
   kind "ConsoleApp"
   language "C"
   targetdir "bin/%{cfg.buildcfg}"

   files { "**.h", "**.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"