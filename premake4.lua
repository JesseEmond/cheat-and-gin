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

	include "CheatEngine"
	include "FakeGame" -- Fake game to test the cheat engine