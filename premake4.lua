solution "cheat_and_gin"
	location "build"
	language "C++"

	configurations { "Debug", "Release" }

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }

	-- platform-specific defines
	if os.is("windows") then
		defines { "_WIN32", "WIN32", "WINDOWS" }
	elseif os.is("linux") then
		defines { "LINUX" }
	end

	include "CheatEngine"
	include "FakeGame" -- Fake game to test the cheat engine