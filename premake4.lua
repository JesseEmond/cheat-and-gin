solution "cheat_and_gin"
	location "build"
	language "C++"
        buildoptions "-std=c++11"

	-- platform-specific defines
	if os.is("windows") then
		defines { "_WIN32", "WIN32", "WINDOWS" }
	elseif os.is("linux") then
		defines { "LINUX" }
	end

	configurations { "Debug", "Release" }

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }

	include "CheatEngine"
	include "FakeGame" -- Fake game to test the cheat engine
