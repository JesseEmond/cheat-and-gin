project "Tests"
	kind "ConsoleApp"

	location "build"
	targetdir "bin"
	objdir "obj"
	files { "../src/*.h", "../src/*.cpp", "./src/*.hpp", "./src/*.cpp" }
        excludes { "../src/*.cpp" }
