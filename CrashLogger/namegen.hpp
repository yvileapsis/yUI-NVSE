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
	"Name Coiner",
	"Crash Logger 2: Electric Boogaloo",
	"Crash Logger: Enhanced Edition",
	"Project Crashlogger",
	"yCL",
	"Fallout-Crashing-Logging Convergence"
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
	"",
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
	" (I'd Rather Be Playing Starfield Edition)",
	" (Downgrade Edition)",
	" (:snig: edition)",
	" (100 endings edition)",
	" (16 times the detail edition)"
	": Roybug Until Proven Otherwise",
	": Just Go Away",
	": At Least With This Upload You Made Some Changes To The Original Mod",
	": FalloutNV.exe has fallen, billion bugs must die",
	": Born To Crash / GameBryo is a F*CK / Fix Em All",
	": I Am Crash Man / 80,101,112,101,103,108,97,114,101 fixed bugs",
	": Just when I thought I was out, they pulled me back in!",
	": Radio Free FalloutNV.exe",
	": Theoretical Degree in Reverse Engineering",
	": Nice Try, Carxt", 
	": FIX IT NOW NOW NOW",
	": Tried Following Viva New Vegas Guide Yet?",
	": ITS NOT A FUCKING HAT",
};

std::string GetName()
{
	std::random_device dev;
	srand(dev());
	return author[std::rand() % author.size()] + names[std::rand() % names.size()] + edition[std::rand() % edition.size()];
}
