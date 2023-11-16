This is a fork of [DLL](https://github.com/DavidJCobb/tes4-crash-logger) with modifications by me and additional contributions by WallSoGB.

This version of Crash Logger comes equipped with a mini-library allowing for on the fly logging of game RTTI classes. It continuously goes through registry and stack and logs any RTTI class it can find.
Additionally, this version picks up PDB information regarding which source files do functions come from.

## How To Use

Main two discrete sections of a crash log are: 1) Calltrace, 2) Registry and Stack.
Calltrace describes *what game was doing* when it crashed, aka where in the tree of called functions it died. To decypher this information you need to either have a .pdb database for the game files or be skilled in reverse-engineering.
Registry and Stack describe *what objects were game operating on* when it crashed. This information is much more hit or miss but it *is* readable by a power-user. In short, the higher up the object is the more relevant it is supposed to be to the crash.
