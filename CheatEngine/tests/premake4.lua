project "Tests"
	kind "ConsoleApp"

	location "build"
	targetdir "bin"
	objdir "obj"
	files { "../src/*.h", "../src/*.cpp",
                "../src/platform/linux/*.cpp", "../src/platform/linux/*.h",
                "src/*.hpp", "src/*.cpp", "src/*.h" }
        excludes { "../src/main.cpp" }
