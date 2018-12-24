-- For documentation on all premake functions please see the wiki:
--   https://github.com/premake/premake-core/wiki

-- Thanks to Johannes Peters for providing the template of this file.


-- Helper function
function RemoveDirs(pattern)
  local matches = os.matchdirs(pattern)
  local count = 0;
  for key,value in pairs(matches) do
    os.rmdir(value)
    count = count + 1 -- sorry, no increment operator in LUA :(
  end
  return count
end

-- Adding new Clean Action
newaction {
  trigger     = "clean",
  description = "clean the build",
  execute     = function ()
    print("cleaning the build...")
    local removed = RemoveDirs(".\\build*") + RemoveDirs(".\\bin_Debug*") + RemoveDirs(".\\bin_Release*")
    os.rmdir(".\\.vs")
    printf("done. %d files removed", removed)
  end
}



workspace "GraphicsEngine"
   configurations { "Debug", "Release" }

   filter { "system:windows", "action:vs*"}
    flags         { "MultiProcessorCompile", "NoMinimalRebuild" }
    linkoptions   { "/ignore:4099", "/NODEFAULTLIB:library" }      -- Ignore library pdb warnings when running in debug
   --pchheader "stdafx.h"
   systemversion( "10.0.15063.0" )
   location("build")
   flags "FatalWarnings" -- comment if you don't want warnings to count as errors
   warnings "Extra"

   characterset ("Unicode")

   filter "configurations:Debug"
    defines { "DEBUG", "_CRT_SECURE_NO_WARNINGS" }
    symbols  "On"
  filter{}

  filter "configurations:Release"
    defines { "NDEBUG", "_CRT_SECURE_NO_WARNINGS" }
    optimize "On"
  filter{}

project "GraphicsEngine!"
   kind "WindowedApp"
   language "C++"
   targetdir "bin_%{cfg.buildcfg}_%{cfg.platform}" -- Where the output binary goes. This will be generated when we build from the makefile/visual studio project/etc.
   targetname "GraphicsEngine"                     -- the name of the executable saved to 'targetdir'

   local SourceDir = "./src/";
   local IncludeDirs = "./inc/";
   files 
   {
      SourceDir .. "**.h",
      SourceDir .. "**.hpp",
      SourceDir .. "**.c",
      SourceDir .. "**.cpp",
      SourceDir .. "**.tpp",

      IncludeDirs .. "**.h",
      IncludeDirs .. "**.hpp",
      IncludeDirs .. "**.c",
      IncludeDirs .. "**.cpp",
      IncludeDirs .. "**.tpp" 
   }
   includedirs
   {
      IncludeDirs
   }

   -- Exclude template files from project (so they don't accidentally get compiled)
    filter { "files:**.tpp" }
      flags {"ExcludeFromBuild"}

    filter {} -- clear filter!


    links {

      -- A list of the actual library/dll names to include
      -- For example if you want to include fmod_123.lib you put "fmod_123" here. Just like when adding to visual studio's linker.
      "d3d11",
      "d3dcompiler",
    }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"