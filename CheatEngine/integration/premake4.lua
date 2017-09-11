project "IntegrationTarget"
	kind "ConsoleApp"

	location "build"
	targetdir "bin"
	objdir "obj"
	files { "src/main_target.cpp" }

project "IntegrationTest"
	kind "ConsoleApp"

	location "build"
	targetdir "bin"
	objdir "obj"
	files { "../src/*.h", "../src/*.cpp",
                "../src/platform/linux/*.cpp", "../src/platform/linux/*.h",
                "src/*.cpp" }
        excludes { "../src/main.cpp",
                   "src/main_target.cpp" }
