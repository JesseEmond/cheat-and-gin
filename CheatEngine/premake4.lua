project "CheatEngine"
	kind "ConsoleApp"

	location "build"
	targetdir "bin"
	objdir "obj"
	files { "./src/*.h", "./src/*.cpp" }
	excludes { "./src/platform/*.cpp" }

	-- platform-specific files
	if os.is("windows") then
		files { "./src/platform/win32/*.cpp" }
        elseif os.is("linux") then
                files { "./src/platform/linux/*.cpp" }
	end
