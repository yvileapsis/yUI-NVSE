#pragma once
#include <random>

inline std::vector<std::string> author{
	"",
	"",
	"",
	"Yvile's ",
	"Yvile's ",
	"Yvile's ",
	"Yvile's Yvile's ",
	"Yvile's Cobb's ",
	"Yvileapsis' ",
	"Yvileapsis' ",
	"Yvileapsis' ",
	"Yvileapsis' Cobb's ",
	"Yvileapsis' Yvile's ",
	"Joe Cobb's ",
	"(Not Really) Cobb's ",
	"Trooper's ",
	"Pepe's ",
	"Todd Howard's ",
	"xNVSE Discord's "
};

inline std::vector<std::string> names{
	"Crash Logger",
	"Crash Logger",
	"Crash Logger",
	"Crash Logger",
	"Crash Logger",
	"Crash Blogger",
	"AAAAAAA IT CRASHED",
	"Obsurdian Ls Logger",
	"Bugthesda Ls Logger",
	"Rash Clogger",
	"Brad Boimler (God, I f*cking hate that show)",
	"Abandonwarebytes",
};

inline std::vector<std::string> edition{
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	": Ultimate Edition",
	" NVSE",
	" (NVSE)",
	" Tweaks",
	" (April's Fools Edition)",
	" (Unfunny Random Names Edition)",
	" (Todd Howard Ruined Fallout Edition)",
	" (Soyvellone and JSoyer Edition)",
	" (Trochili Edition)",
	" (Skill Issue Edition)",
	" (Works On My Machine Edition)"
};

std::string GetName()
{
	std::random_device dev;
	srand(dev());
	return author[std::rand() % author.size()] + names[std::rand() % names.size()] + edition[std::rand() % edition.size()];
}
